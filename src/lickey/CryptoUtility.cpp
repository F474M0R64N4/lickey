#include "stdafx.h"
#include "CryptoUtility.h"

#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"
#include "mbedcrypto/cipher.hpp"

using namespace mbedcrypto;
using namespace std;

namespace lickey
{
	auto Encrypt(const std::string& data, const size_t datalen, std::string key, std::string iv, std::string& dest,
	             size_t& destlen) -> bool
	{
		const auto ciphered_buffer = cipher::encrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key.append(key), data);
		dest = ciphered_buffer;
		return true;
	}

	auto Decrypt(const std::string& data, const size_t datalen, std::string key, std::string iv, std::string& dest,
	             size_t& destlen) -> bool
	{
		const auto decrypted_buffer = cipher::decrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key.append(key), data);
		dest = decrypted_buffer;
		return true;
	}

	auto MD5(std::string data, size_t datalen, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::md5, data);
		datalen = data.length();
		return true;
	}

	auto SHA256(std::string& data, size_t datalen, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::sha256, data);
		return false;
	}

	auto EncodeBase64(std::string& data, std::string& str) -> void
	{
		str = to_base64(data);
	}

	auto DecodeBase64(std::string& str, std::string& data, int& datalen) -> void
	{
		data = base64::decode(str);
		datalen = str.length();
	}


	auto MakeSalt(Salt& salt) -> bool
	{
		std::string tmp;
		std::string encoded;

		// TODO: заглушка
		if (tmp.empty())
		{
			tmp = "salt";
		}

		EncodeBase64(tmp, encoded);
		salt = encoded;
		return true;
	}
}
