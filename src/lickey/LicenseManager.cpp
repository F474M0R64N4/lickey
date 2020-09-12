#include "stdafx.h"
#include "LicenseManager.h"
#include <algorithm>
#include <fstream>
#include <utility>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include "CryptoUtility.h"
#include "FileUtility.h"
#include "HardwareKeyGetter.h"
#include "Version.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace lickey;

namespace
{
	const unsigned int buf_size = 65536;
	const std::string data_section_delimiter = "***";

	using el = struct encrypt_license
	{
		hardware_key key;
		std::string vendor_name;
		std::string app_name;
		hash first_feature_sign;
		salt explicit_salt;
		salt implicit_salt;
		date last_used_date;
	};

	using dl = struct decrypt_license
	{
		hardware_key key;
		std::string vendor_name;
		std::string app_name;
		hash first_feature_sign;
		salt explicit_salt;
	};

	auto into_char = [](const unsigned char c)
	{
		return static_cast<char>(c);
	};

	auto calc_base64_encoded_size(const int orig_data_size) -> int
	{
		const auto num_blocks6 = (orig_data_size * 8 + 5) / 6; // the number of blocks (6 bits per a block, rounding up)
		const auto num_blocks4 = (num_blocks6 + 3) / 4; // the number of blocks (4 characters per a block, rounding up)
		const auto num_net_chars = num_blocks4 * 4; // the number of characters without carriage return
		return num_net_chars + ((num_net_chars / 76) * 2);
		// the number of encoded characters (76 characters per line, currently only for carriage return)
	}


	auto convert(const std::string& feature_name, const feature_info& feature_info) -> std::string
	{
		std::string converted;
		converted.append("feature ");
		converted.append("name=").append(feature_name).append(" ");
		converted.append("version=").append(feature_info.version().value()).append(" ");
		converted.append("issue=").append(to_string(feature_info.issue_date())).append(" ");
		converted.append("expire=").append(to_string(feature_info.expire_date())).append(" ");
		converted.append("num=");
		converted.append(std::to_string(feature_info.num_lics()));
		converted.append(" ");
		converted.append("sign=").append(feature_info.sign().value());

		return converted;
	}
}

namespace
{
	using feature_tree = std::map<std::string, std::string>;
	using ft_itr = feature_tree::iterator;

	auto split(const std::string& line, std::vector<std::string>& tokens, const std::string& delim = " ") -> void
	{
		const auto trim = [](std::string& str)
		{
			boost::trim(str);
		};
		split(tokens, line, boost::is_any_of(delim));
		boost::for_each(tokens, trim);
	}

	auto make_feature_tree(
		const std::vector<std::string>& tokens,
		feature_tree& tree) -> void
	{
		for (const auto& token : tokens)
		{
			std::vector<std::string> sub_tokens;
			split(token, sub_tokens, "=");

			if (2 > sub_tokens.size())
			{
				if ("feature" == sub_tokens.front())
				{
					tree["feature"] = "feature";
				}

				continue;
			}

			if ("sign" == sub_tokens.front())
			{
				// because sign value can have "=",
				tree["sign"] = token.substr(5, token.size() - 5);
			}
			else
			{
				auto key = sub_tokens.front();
				std::transform(key.begin(), key.end(), key.begin(), tolower);
				tree[key] = sub_tokens[1];
			}
		}
	}


	auto find_data_section(
		const std::vector<std::string>& lines,
		std::string& data) -> bool
	{
		// проверим разделитель в строке
		const auto is_data_delimiter = [](const std::string& line)
		{
			const auto pos = line.find_first_of(data_section_delimiter);

			if (std::string::npos == pos)
			{
				return false;
			}

			if (0 != pos)
			{
				return false;
			}

			return true;
		};
		auto does_data_exist = false;
		auto is_in_data = false;
		std::string data_stream; // буфер под лицензию

		for (const auto& line : lines)
		{
			// проверка
			if (is_data_delimiter(line))
			{
				is_in_data = !is_in_data;

				if (is_in_data)
				{
					continue;
				}

				break;
			}

			if (is_in_data)
			{
				data_stream.append(line); // запишем строки секции data в буфер
				does_data_exist = true; // секция data найдена
			}
		}

		data = data_stream;
		return does_data_exist; // возвратим флаг
	}

	auto make_encryption_key(
		const hardware_key& key,
		const std::string& vendor_name,
		const std::string& app_name,
		const hash& first_feature_sign,
		const salt& explicit_salt,
		std::string& encryption_key) -> bool
	{
		std::string src;
		src.append(key.value()).append(explicit_salt.value()).append(vendor_name).append(app_name).append(
			first_feature_sign.value());
		md5(src, encryption_key);
		return true;
	}


	auto make_encryption_iv(
		const hardware_key& key,
		const salt& explicit_salt,
		std::string& encryption_key,
		std::string& encryption_iv) -> bool
	{
		std::string encoded_key;
		encode_base64(encryption_key, encoded_key);
		std::string src;
		src.append(encoded_key).append(key.value()).append(explicit_salt.value());
		//MD5(src.str().c_str(), src.str().size(), encryptionIv);
		md5(src, encryption_iv);
		return true;
	}


	auto make_feature_sign(
		const std::string& feature_name,
		const feature_info& feature_info,
		const salt& implicit_salt,
		hash& sign) -> bool
	{
		std::string src;
		src.append(feature_name).append(feature_info.version().value()).append(to_string(feature_info.issue_date())).
		    append(
			    to_string(feature_info.expire_date())).append(std::to_string(feature_info.num_lics())).append(
			    implicit_salt.value());
		std::string sha;
		sha256(src, sha);
		std::string encoded_sign;
		encode_base64(sha, encoded_sign);
		sign = encoded_sign;
		return true;
	}


	auto decrypt_data(
		const dl& dl,
		salt& implicit_salt,
		date& last_used_date,
		const std::string& data
	) -> bool
	{
		// буфер для ключа расшифровки
		std::string encryption_key;

		// создадим ключ расшифровки
		if (make_encryption_key(dl.key, dl.vendor_name, dl.app_name, dl.first_feature_sign, dl.explicit_salt,
		                        encryption_key))
		{
			// буфер для ключа инициализации
			std::string encryption_iv;

			// создадим ключ инициализации
			if (make_encryption_iv(dl.key, dl.explicit_salt, encryption_key, encryption_iv))
			{
				// буфер для расшифрованных данных
				//unsigned char decryptedImpl[BUF_SIZE] = {'\0'};
				std::string decrypted_impl;
				//размер расшифрованных данных
				const size_t decrypted_impl_size = buf_size;
				// расшифровываем данные
				decrypt(data, encryption_key, encryption_iv, decrypted_impl);
				const auto valid_len = calc_base64_encoded_size(4) + 8;

				if (static_cast<size_t>(valid_len) > decrypted_impl_size)
				{
					LOG(error) << "invalid data section";
					return false;
				}

				const auto salt_impl = decrypted_impl.substr(0, 8);
				implicit_salt = salt_impl;

				// дата лицензии
				const auto date_impl = decrypted_impl.substr(8, 16);

				if (load(last_used_date, date_impl))
				{
					return true;
				}
				LOG(error) << "fail to decrypt date because of invalid date";
				return false;
			}
			LOG(error) << "fail to get iv";
			return false;
		}
		LOG(error) << "fail to get key";
		return false;
	}


	auto encrypt_data(const el& el, std::string& encrypted) -> bool
	{
		std::string encryption_key;

		if (make_encryption_key(el.key, el.vendor_name, el.app_name, el.first_feature_sign, el.explicit_salt,
		                        encryption_key))
		{
			std::string encryption_iv;

			if (make_encryption_iv(el.key, el.explicit_salt, encryption_key, encryption_iv))
			{
				const auto str_date = to_string(el.last_used_date);
				assert(8 == str_date.size());
				// base64 salt + date
				const auto dst = el.implicit_salt.value().append(str_date);
				std::string ecrypted_impl;
				encrypt(dst, encryption_key, encryption_iv, ecrypted_impl);
				encode_base64(ecrypted_impl, encrypted);
				return true;
			}
			LOG(error) << "fail to get iv";
			return false;
		}
		LOG(error) << "fail to get key";
		return false;
	}
}


namespace lickey
{
	license_manager::license_manager(std::string vn, std::string an) : vendor_name_(std::move(vn)),
	                                                                   app_name_(std::move(an)),
	                                                                   is_license_loaded_(false)
	{
	}


	license_manager::~license_manager()
	{
		update();
	}


	auto license_manager::is_license_decrypt(const hardware_key& key, license& license,
	                                         std::string& decoded) -> bool
	{
		dl decrypt_license; // структура дешифрованной лицензии
		decrypt_license.key = key;
		decrypt_license.vendor_name = vendor_name_;
		decrypt_license.app_name = app_name_;
		decrypt_license.first_feature_sign = license.features_.begin()->second.sign_;
		decrypt_license.explicit_salt = license.explicit_salt_;

		if (decrypt_data(decrypt_license, license.implicit_salt_, license.last_used_date_, decoded))
		{
			// validate each feature
			for (auto cit = license.features_.begin(); cit != license.features_.end(); ++cit)
			{
				hash sign;
				make_feature_sign(cit->first, cit->second, license.implicit_salt_, sign);
				cit->second.check_sum_ = sign;
			}

			loaded_license_ = license;
			is_license_loaded_ = true;
			return true;
		}

		LOG(error) << "fail to decrypt";
		return false;
	}

	auto license_manager::is_license_data_section_read(const hardware_key& key, license& license,
	                                                   const std::vector<std::string>& lines) -> bool
	{
		// буфер под лицензию
		std::string data;

		if (find_data_section(lines, data))
		{
			std::string decoded; // дешифрованная лицензия
			decode_base64(data, decoded);

			// read from json
			auto j = json::parse(decoded);

			std::string version = j["version"];
			std::string encrypted = j["encrypted"];

			license.explicit_salt_ = j["explicit_salt"];

			std::string buffer; // буфер под дешифрованную лицензию
			decode_base64(encrypted, buffer); // дешифруем лицензию из base64 формата

			return is_license_decrypt(key, license, buffer); // дешифруем лицензию
		}

		LOG(error) << "no data sections"; // ошибка мол нет секции лицензии
		return false;
	}

	auto license_manager::is_license_read(const std::string& filepath, const hardware_key& key,
	                                      license& license) -> bool
	{
		std::vector<std::string> lines;

		if (read_lines(filepath, lines))
		{
			// load features section
			for (auto& line : lines)
			{
				std::string feature_name;
				feature_info feature_info;

				if (convert_feature(line, feature_name, feature_info))
				{
					license.features_[feature_name] = feature_info;
				}
			}

			if (license.features_.empty())
			{
				LOG(error) << "no feature";
				return false;
			}

			// load date section
			return is_license_data_section_read(key, license, lines);
		}

		LOG(error) << "fail to open";
		return false;
	}

	auto license_manager::load(const std::string& filepath, const hardware_key& key, license& license) -> bool
	{
		license_filepath_ = filepath;
		is_license_loaded_ = false;
		license.key_ = key;
		LOG(info) << "start to load license file = " << filepath;
		return is_license_read(filepath, key, license);
	}

	auto license_manager::update() -> bool
	{
		if (is_license_loaded_)
		{
			if (loaded_license_.features_.empty())
			{
				LOG(error) << "no feature to generate license file";
				return false;
			}

			make_salt(loaded_license_.explicit_salt_);
			make_salt(loaded_license_.implicit_salt_);

			for (auto& feature : loaded_license_.features_)
			{
				hash sign;
				make_feature_sign(feature.first, feature.second, loaded_license_.implicit_salt_, sign);
				feature.second.sign_ = sign;
			}

			return update_license();
		}

		LOG(error) << "license is not loaded";
		return false;
	}

	auto license_manager::update_license() -> bool
	{
		std::string encrypted; // зашифрованная лицензия
		date today;
		set_today(today); // назначим теперешнюю дату
		el encrypt_license;
		encrypt_license.key = loaded_license_.key_; // hwid ПК
		encrypt_license.vendor_name = vendor_name_; // vendor
		encrypt_license.app_name = app_name_; // имя программы
		encrypt_license.first_feature_sign = loaded_license_.features_.begin()->second.sign_; // контрольная сумма
		encrypt_license.explicit_salt = loaded_license_.explicit_salt_;
		encrypt_license.implicit_salt = loaded_license_.implicit_salt_;
		encrypt_license.last_used_date = today; // когда использовали последний раз лицензию

		// зашифруем данные
		if (encrypt_data(encrypt_license, encrypted))
		{
			std::ostringstream data_section(std::ios::binary);
			std::string file_version = LICENSE_VERSION();
			const auto explicit_salt_value = loaded_license_.explicit_salt_.value();
			//data_section.write(static_cast<const char*>(&file_version), sizeof(unsigned int));
			//data_section.write(explicit_salt_value.c_str(), sizeof(char) * explicit_salt_value.size());
			//data_section.write(encrypted.c_str(), sizeof(char) * encrypted.size());
			// make json string
			json ds;
			ds["version"] = file_version;
			ds["explicit_salt"] = explicit_salt_value;
			ds["encrypted"] = encrypted;
			std::string ds_string;
			ds_string.append(ds.dump());
			encode_base64(ds_string, encrypted);
			// запись в файл
			std::ofstream out(license_filepath_.c_str());

			if (out)
			{
				for (const auto& feature : loaded_license_.features_)
				{
					out << convert(feature.first, feature.second) << "\n";
				}

				// запишем обратно в файл новые данные
				out << "\n";
				out << data_section_delimiter << "\n";
				out << encrypted << "\n";
				out << data_section_delimiter << "\n";
				out.close();
				return true;
			}

			LOG(error) << "fail to open = " << license_filepath_; // вывод ошибки открытия файла
			return false;
		}

		// LOG(error) << "fail to make data section"; // ошибка создания секции "data"
		return false;
	}

	auto license_manager::save(const std::string& filepath, const hardware_key& key, license& license) -> bool
	{
		license_filepath_ = filepath;
		loaded_license_ = license;
		loaded_license_.key_ = key;
		is_license_loaded_ = true;
		return update();
	}


	void license_manager::add(
		const std::string& feature_name,
		const feature_version& feature_version,
		const date& issue_date,
		const date& expire_date,
		const unsigned int num_lics,
		license& license)
	{
		feature_info info;
		info.version_ = feature_version;
		info.issue_date_ = issue_date;
		info.expire_date_ = expire_date;
		info.num_lics_ = num_lics;
		license.features_[feature_name] = info;
	}


	auto license_manager::convert_feature(
		const std::string& line,
		std::string& feature_name,
		feature_info& feature_info) -> bool
	{
		std::vector<std::string> tokens; // храним тут все токены
		split(line, tokens); // из строки вытаскиваем все токены

		// если токенов нет - разворачиваем обратно
		if (tokens.empty())
		{
			return false;
		}

		feature_tree tree; // дерево данных лицензии
		make_feature_tree(tokens, tree); // преобразуем токены в дерево
		auto it = tree.find("feature");

		if (tree.end() == it)
		{
			return false;
		}

		it = tree.find("name"); // имя лицензии

		if (tree.end() == it)
		{
			LOG(error) << "name not found in feature line (name = " << feature_name << ")\n";
			return false;
		}

		feature_name = it->second;
		it = tree.find("version"); // версия лицензии

		if (tree.end() == it)
		{
			LOG(error) << "version not found in feature line (name = " << feature_name << ")\n";
			return false;
		}

		feature_info.version_.version_ = it->second;
		it = tree.find("issue"); // когда выдана лицензия

		if (tree.end() == it)
		{
			LOG(error) << "issue not found in feature line (name = " << feature_name << ")\n";
			return false;
		}

		if (lickey::load(feature_info.issue_date_, it->second))
		{
			it = tree.find("expire"); // конец даты лицензии

			if (tree.end() == it)
			{
				LOG(error) << "expire not found in feature line (name = " << feature_name << ")\n";
				return false;
			}

			if (lickey::load(feature_info.expire_date_, it->second))
			{
				it = tree.find("num"); // количество выданных лицензий

				if (tree.end() == it)
				{
					LOG(error) << "num not found in feature line (name = " << feature_name << ")\n";
					return false;
				}

				feature_info.num_lics_ = boost::lexical_cast<int>(it->second);
				it = tree.find("sign"); // контрольная сумма лицензии

				if (tree.end() == it)
				{
					LOG(error) << "sign not found in feature line (name = " << feature_name << ")\n";
					return false;
				}

				feature_info.sign_ = it->second;
				LOG(info) << "done to convert feature successfully (name = " << feature_name << ")\n";
				return true;
			}
			LOG(error) << "invalid expire date = " << it->second << " (name = " << feature_name << ")\n";
			return false;
		}
		LOG(error) << "invalid issue date = " << it->second << " (name = " << feature_name << ")\n";
		return false;
	}
}
