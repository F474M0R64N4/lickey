#include "stdafx.h"
#include "LicenseManager.h"
#include <algorithm>
#include <fstream>
#include <utility>
#include <boost/scoped_array.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include "CryptoUtility.h"
#include "FileUtility.h"
#include "HardwareKeyGetter.h"
#include "Version.h"

namespace {
  using namespace lickey;

  const unsigned int BUF_SIZE = 65536;
  const std::string DATA_SECTION_DELIMITER = "***";

  using EL = struct EncryptLicense {
    HardwareKey key;
    std::string vendorName;
    std::string appName;
    Hash firstFeatureSign;
    Salt explicitSalt;
    Salt implicitSalt;
    Date lastUsedDate;
  };

  using DL = struct DecryptLicense {
    /**
     * \brief HWID
     */
    HardwareKey key;
    /**
     * \brief Vendor
     */
    std::string vendorName;
    /**
     * \brief Application Name
     */
    std::string appName;
    /**
     * \brief First Feature checksum
     */
    Hash firstFeatureSign;
    /**
     * \brief Explicit salt
     */
    Salt explicitSalt;
  };

  struct UnsignedChar2Char {
    auto operator()(unsigned char c) const -> char {
      return static_cast<char>(c);
    }
  };

  auto IntoChar = [](unsigned char c) {
    return static_cast<char>(c);
  };

  auto CalcBase64EncodedSize(int origDataSize) -> int {
    const int numBlocks6 = (origDataSize * 8 + 5) / 6; // the number of blocks (6 bits per a block, rounding up)
    const int numBlocks4 = (numBlocks6 + 3) / 4; // the number of blocks (4 characters per a block, rounding up)
    const int numNetChars = numBlocks4 * 4; // the number of characters without carriage return
    return numNetChars + ((numNetChars / 76) * 2);
    // the number of encoded characters (76 characters per line, currently only for carriage return)
  }


  auto Convert(const std::string &featureName, const FeatureInfo &featureInfo) -> std::string {
  		std::string converted;
		converted.append("feature ");
		converted.append("name=").append(featureName).append(" ");
		converted.append("version=").append(featureInfo.Version().Value()).append(" ");
		converted.append("issue=").append(ToString(featureInfo.IssueDate())).append(" ");
		converted.append("expire=").append(ToString(featureInfo.ExpireDate())).append(" ");
		converted.append("num=");
		converted.append(std::to_string(featureInfo.NumLics()));
		converted.append(" ");
		converted.append("sign=").append(featureInfo.Sign().Value());

		return converted;
  }
}

namespace {
  using FeatureTree = std::map<std::string, std::string>;
  using FTItr = FeatureTree::iterator;


  void Split(const std::string &line, std::vector<std::string> &tokens, const std::string &delim = " ") {
    const auto trim = [](std::string & str) {
      boost::trim(str);
    };
    split(tokens, line, boost::is_any_of(delim));
    boost::for_each(tokens, trim);
  }


  void MakeFeatureTree(
    const std::vector<std::string> &tokens,
    FeatureTree &tree) {
    for (const auto &token : tokens) {
      std::vector<std::string> subTokens;
      Split(token, subTokens, "=");

      if (2 > subTokens.size()) {
        if ("feature" == subTokens.front()) {
          tree["feature"] = "feature";
        }

        continue;
      }

      if ("sign" == subTokens.front()) {
        // because sign value can have "=",
        tree["sign"] = token.substr(5, token.size() - 5);

      } else {
        std::string key = subTokens.front();
        std::transform(key.begin(), key.end(), key.begin(), tolower);
        tree[key] = subTokens[1];
      }
    }
  }


  auto FindDataSection(
    const std::vector<std::string> &lines,
    std::string &data) -> bool {
    // проверим разделитель в строке
    const auto isDataDelimiter = [](const std::string & line) {
      const std::string::size_type pos = line.find_first_of(DATA_SECTION_DELIMITER);

      if (std::string::npos == pos) {
        return false;
      }

      if (0 != pos) {
        return false;
      }

      return true;
    };
    bool doesDataExist = false;
    bool isInData = false;
    std::string dataStream; // буфер под лицензию

    for (const auto &line : lines) {
      // проверка
      if (isDataDelimiter(line)) {
        isInData = !isInData;

        if (!isInData) {
          break;
        }

        continue;
      }

      if (isInData) {
        dataStream.append(line); // запишем строки секции data в буфер
        doesDataExist = true; // секция data найдена
      }
    }

    data = dataStream;
    return doesDataExist; // возвратим флаг
  }

  auto MakeEncryptionKey(
    const HardwareKey &key,
    const std::string &vendorName,
    const std::string &appName,
    const Hash &firstFeatureSign,
    const Salt &explicitSalt,
    std::string &encryptionKey) -> bool {
    std::string src;
    src.append(key.Value()).append(explicitSalt.Value()).append(vendorName).append(appName).append(firstFeatureSign.Value());
    //MD5(src.str().c_str(), src.str().size(), encryptionKey);
    MD5(src, encryptionKey);
    return true;
  }


  auto MakeEncryptionIv(
    const HardwareKey &key,
    const Salt &explicitSalt,
    std::string &encryptionKey,
    std::string &encryptionIv) -> bool {
    std::string encodedKey;
    EncodeBase64(encryptionKey, encodedKey);
    std::string src;
    src.append(encodedKey).append(key.Value()).append(explicitSalt.Value());
    //MD5(src.str().c_str(), src.str().size(), encryptionIv);
    MD5(src, encryptionIv);
    return true;
  }


  auto MakeFeatureSign(
    const std::string &featureName,
    const FeatureInfo &featureInfo,
    const Salt &implicitSalt,
    Hash &sign) -> bool {
    std::string src;
    src.append(featureName).append( featureInfo.Version().Value()).append(ToString(featureInfo.IssueDate())).append(
      ToString(featureInfo.ExpireDate())).append( std::to_string(featureInfo.NumLics())).append(implicitSalt.Value());
    std::string sha;
    SHA256(src, sha);
    std::string encodedSign;
    EncodeBase64(sha, encodedSign);
    sign = encodedSign;
    return true;
  }


  auto DecryptData(
    const DL &dl,
    Salt &implicitSalt,
    Date &lastUsedDate,
    const std::string& data,
    size_t datalen
  ) -> bool {
    // буфер для ключа расшифровки
    std::string encryptionKey;

    // создадим ключ расшифровки
    if (!MakeEncryptionKey(dl.key, dl.vendorName, dl.appName, dl.firstFeatureSign, dl.explicitSalt, encryptionKey)) {
      LOG(error) << "fail to get key";
      return false;
    }

    // буфер для ключа инициализации
    std::string encryptionIv;

    // создадим ключ инициализации
    if (!MakeEncryptionIv(dl.key, dl.explicitSalt, encryptionKey, encryptionIv)) {
      LOG(error) << "fail to get iv";
      return false;
    }

    // буфер для расшифрованных данных
    //unsigned char decryptedImpl[BUF_SIZE] = {'\0'};
    std::string decryptedImpl;
    //размер расшифрованных данных
    size_t decryptedImplSize = BUF_SIZE;
    // расшифровываем данные
    // Decrypt(data, datalen, encryptionKey, encryptionIv, decryptedImpl, decryptedImplSize);
    Decrypt(data, encryptionKey, encryptionIv, decryptedImpl, decryptedImplSize);
    //char *decryptedImplChar = static_cast<char *>(malloc(decryptedImplSize));
    //boost::scoped_array<char> scopedDecryptedImplChar(decryptedImplChar);
    //std::transform(decryptedImpl.c_str(), decryptedImpl.c_str() + decryptedImplSize, decryptedImplChar, UnsignedChar2Char());
    std::istringstream src(decryptedImpl, std::ios::binary);
    const int validLen = CalcBase64EncodedSize(4) + 8;

    if (static_cast<size_t>(validLen) > decryptedImplSize) {
      LOG(error) << "invalid data section";
      return false;
    }

    // соль
    char *saltImpl = static_cast<char *>(malloc(
          static_cast<size_t>(sizeof(char) * static_cast<size_t>(CalcBase64EncodedSize(4)) + 1)));

    if (saltImpl == nullptr) {
      assert(saltImpl);

    } else {
      boost::scoped_array<char> scopedSaltImpl(saltImpl);
      src.read(saltImpl, static_cast<int>(sizeof(char)) * CalcBase64EncodedSize(4));
      const auto it = static_cast<size_t>(CalcBase64EncodedSize(4)); //memsize
      saltImpl[it] = '\0';
      implicitSalt = saltImpl;
    }

    // дата лицензии
    char *dateImpl = static_cast<char *>(malloc(sizeof(char) * 8 + 1));

    if (dateImpl == nullptr) {
      assert(dateImpl);

    } else {
      boost::scoped_array<char> scopedDateImpl(dateImpl);
      src.read(dateImpl, sizeof(char) * 8);
      dateImpl[8] = '\0';

      if (!Load(lastUsedDate, dateImpl)) {
        LOG(error) << "fail to decrypt date because of invalid date";
        return false;
      }

      return true;
    }

    return false;
  }


  auto EncryptData(const EL &el, std::string &encrypted) -> bool {
    std::string encryptionKey;

    if (!MakeEncryptionKey(el.key, el.vendorName, el.appName, el.firstFeatureSign, el.explicitSalt, encryptionKey)) {
      LOG(error) << "fail to get key";
      return false;
    }

    std::string encryptionIv;

    if (!MakeEncryptionIv(el.key, el.explicitSalt, encryptionKey, encryptionIv)) {
      LOG(error) << "fail to get iv";
      return false;
    }

    const std::string strDate = ToString(el.lastUsedDate);
    assert(8 == strDate.size());
    std::ostringstream dst(std::ios::binary);
    dst.write(el.implicitSalt.Value().c_str(), sizeof(char) * el.implicitSalt.Value().size());
    dst.write(strDate.c_str(), sizeof(char) * strDate.size());
    std::string ecryptedImpl;
    size_t ecryptedImplSize = BUF_SIZE;
    //Encrypt(dst.str().c_str(), dst.str().size(), encryptionKey, encryptionIv, ecryptedImpl, ecryptedImplSize);
    Encrypt(dst.str(), encryptionKey, encryptionIv, ecryptedImpl, ecryptedImplSize);
    /////////////
    EncodeBase64(ecryptedImpl, encrypted);
    return true;
  }
}


namespace lickey {
  LicenseManager::LicenseManager(std::string vn, std::string an) : vendorName(std::move(vn)), appName(std::move(an)),
    isLicenseLoaded(false) {
  }


  LicenseManager::~LicenseManager() {
    Update();
  }


  auto LicenseManager::isLicenseDecrypt(const HardwareKey &key, License &license, int decoded_size,
    std::string &decoded) -> bool {
    DL decrypt_license; // структура дешифрованной лицензии
    decrypt_license.key = key;
    decrypt_license.vendorName = vendorName;
    decrypt_license.appName = appName;
    decrypt_license.firstFeatureSign = license.features.begin()->second.sign;
    decrypt_license.explicitSalt = license.explicitSalt;

    if (DecryptData(decrypt_license, license.implicitSalt, license.lastUsedDate,
        decoded,
        static_cast<const size_t>(decoded_size)
      )) {
      // validate each feature
      for (auto cit = license.features.begin(); cit != license.features.end(); ++cit) {
        Hash checkSum;
        MakeFeatureSign(cit->first, cit->second, license.implicitSalt, checkSum);
        cit->second.checkSum = checkSum;
      }

      loadedLicense = license;
      isLicenseLoaded = true;
      return true;
    }

    LOG(error) << "fail to decrypt";
    return false;
  }

  auto LicenseManager::isLicenseDataSectionRead(const HardwareKey &key, License &license,
    const std::vector<std::string> &lines) -> bool {
    // буфер под лицензию
    std::string data;

    if (FindDataSection(lines, data)) {
      int decodedSize = 0;
      std::string decoded; // дешифрованная лицензия
      DecodeBase64(data, decoded, decodedSize);
      // boost::scoped_array<unsigned char> scopedDecoded(decoded.c_str());
      /*   if (36 > decodedSize) {
           LOG(error) << "no information in data section";
           return false;
         }*/
      std::string dataBuffer(decodedSize, '\0');
      std::transform(decoded.c_str(), decoded.c_str() + static_cast<unsigned char>(decodedSize), dataBuffer.begin(), IntoChar);
      std::istringstream dataSection(dataBuffer, std::ios::binary);
      dataSection.read(static_cast<char *>(&license.fileVersion), sizeof(unsigned int));
      const int saltLengthInBase64 = CalcBase64EncodedSize(4);
      // соль лицензии
      char *salt = static_cast<char *>(malloc(
            static_cast<size_t>(sizeof(char) * static_cast<size_t>(saltLengthInBase64) + 1)));

      if (salt == nullptr) {
        assert(salt);

      } else {
        boost::scoped_array<char> scopedSaltImpl(salt);
        dataSection.read(salt, static_cast<int>(sizeof(char)) * saltLengthInBase64);
        const auto it = static_cast<size_t>(saltLengthInBase64); //memsize
        salt[it] = '\0';
        // "точная" соль лицензии
        license.explicitSalt = salt;
      }

      // оставшееся количество символов в лицензии
      const int remainLen = decodedSize - saltLengthInBase64 - static_cast<int>(sizeof(unsigned int));

      if (1 > remainLen) {
        LOG(error) << "no encrypted data in data section";
        return false;
      }

      // выделим память остальные данные лицензии
      char *base64Encrypted = static_cast<char *>(malloc(
            static_cast<size_t>(sizeof(char) * static_cast<size_t>(remainLen) + 1)));

      if (base64Encrypted == nullptr) {
        assert(base64Encrypted);

      } else {
        boost::scoped_array<char> scpdBase64Encrypted(base64Encrypted);
        dataSection.read(base64Encrypted, static_cast<int>(sizeof(char)) * remainLen);
        const auto it = static_cast<size_t>(remainLen); //memsize
        base64Encrypted[it] = '\0';
        int decoded_size = 0;
        std::string decoded_; // буфер под дешифрованную лицензию
      	std::string bse(base64Encrypted);
        DecodeBase64(bse, decoded_, decoded_size); // дешифруем лицензию из base64 формата
        //boost::scoped_array<unsigned char> scopedDecoded(decoded);
        return isLicenseDecrypt(key, license, decoded_size, decoded_); // дешифруем лицензию
      }

      return false;
    }

    LOG(error) << "no data sections"; // ошибка мол нет секции лицензии
    return false;
  }

  auto LicenseManager::isLicenseRead(const std::string &filepath, const HardwareKey &key, License &license) -> bool {
    std::vector<std::string> lines;

    if (ReadLines(filepath, lines)) {
      // load features section
      for (auto &line : lines) {
        std::string featureName;
        FeatureInfo featureInfo;

        if (ConvertFeature(line, featureName, featureInfo)) {
          license.features[featureName] = featureInfo;
        }
      }

      if (license.features.empty()) {
        LOG(error) << "no feature";
        return false;
      }

      // load date section
      return isLicenseDataSectionRead(key, license, lines);
    }

    LOG(error) << "fail to open";
    return false;
  }

  auto LicenseManager::Load(const std::string &filepath, const HardwareKey &key, License &license) -> bool {
    licenseFilepath = filepath;
    isLicenseLoaded = false;
    license.key = key;
    LOG(info) << "start to load license file = " << filepath;
    return isLicenseRead(filepath, key, license);
  }

  auto LicenseManager::Update() -> bool {
    if (isLicenseLoaded) {
      if (loadedLicense.features.empty()) {
        LOG(error) << "no feature to generate license file";
        return false;
      }

      MakeSalt(loadedLicense.explicitSalt);
      MakeSalt(loadedLicense.implicitSalt);

      for (auto &feature : loadedLicense.features) {
        Hash sign;
        MakeFeatureSign(feature.first, feature.second, loadedLicense.implicitSalt, sign);
        feature.second.sign = sign;
      }

      return UpdateLicense();
    }

    LOG(error) << "license is not loaded";
    return false;
  }

  auto LicenseManager::UpdateLicense() -> bool {
    std::string encrypted; // зашифрованная лицензия
    Date today;
    SetToday(today); // назначим теперешнюю дату
    EL encrypt_license;
    encrypt_license.key = loadedLicense.key; // hwid ПК
    encrypt_license.vendorName = vendorName; // vendor
    encrypt_license.appName = appName; // имя программы
    encrypt_license.firstFeatureSign = loadedLicense.features.begin()->second.sign; // контрольная сумма
    encrypt_license.explicitSalt = loadedLicense.explicitSalt;
    encrypt_license.implicitSalt = loadedLicense.implicitSalt;
    encrypt_license.lastUsedDate = today; // когда использовали последний раз лицензию

    // зашифруем данные
    if (EncryptData(encrypt_license, encrypted)) {
      std::ostringstream dataSection(std::ios::binary);
      char fileVersion = VERSION();
      std::string explictSaltValue = loadedLicense.explicitSalt.Value();
      dataSection.write(static_cast<const char *>(&fileVersion), sizeof(unsigned int));
      dataSection.write(explictSaltValue.c_str(), sizeof(char) * explictSaltValue.size());
      dataSection.write(encrypted.c_str(), sizeof(char) * encrypted.size());
      EncodeBase64(dataSection.str(), encrypted);
      // запись в файл
      std::ofstream out(licenseFilepath.c_str());

      if (out) {
        for (const auto &feature : loadedLicense.features) {
          out << Convert(feature.first, feature.second) << "\n";
        }

        // запишем обратно в файл новые данные
        out << "\n";
        out << DATA_SECTION_DELIMITER << "\n";
        out << encrypted << "\n";
        out << DATA_SECTION_DELIMITER << "\n";
        out.close();
        return true;
      }

      LOG(error) << "fail to open = " << licenseFilepath; // вывод ошибки открытия файла
      return false;
    }

   // LOG(error) << "fail to make data section"; // ошибка создания секции "data"
    return false;
  }

  auto LicenseManager::Save(const std::string &filepath, const HardwareKey &key, License &license) -> bool {
    licenseFilepath = filepath;
    loadedLicense = license;
    loadedLicense.key = key;
    isLicenseLoaded = true;
    return Update();
  }


  void LicenseManager::Add(
    const std::string &featureName,
    const FeatureVersion &featureVersion,
    const Date &issueDate,
    const Date &expireDate,
    unsigned int numLics,
    License &license) {
    FeatureInfo featureInfo;
    featureInfo.version = featureVersion;
    featureInfo.issueDate = issueDate;
    featureInfo.expireDate = expireDate;
    featureInfo.numLics = numLics;
    license.features[featureName] = featureInfo;
  }


  auto LicenseManager::ConvertFeature(
    const std::string &line,
    std::string &featureName,
    FeatureInfo &featureInfo) -> bool {
    std::vector<std::string> tokens; // храним тут все токены
    Split(line, tokens); // из строки вытаскиваем все токены

    // если токенов нет - разворачиваем обратно
    if (tokens.empty()) {
      return false;
    }

    FeatureTree featureTree; // дерево данных лицензии
    MakeFeatureTree(tokens, featureTree); // преобразуем токены в дерево
    auto it = featureTree.find("feature");

    if (featureTree.end() == it) {
      return false;
    }

    it = featureTree.find("name"); // имя лицензии

    if (featureTree.end() == it) {
      LOG(error) << "name not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    featureName = it->second;
    it = featureTree.find("version"); // версия лицензии

    if (featureTree.end() == it) {
      LOG(error) << "version not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    featureInfo.version.version = it->second;
    it = featureTree.find("issue"); // когда выдана лицензия

    if (featureTree.end() == it) {
      LOG(error) << "issue not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    if (!lickey::Load(featureInfo.issueDate, it->second)) {
      LOG(error) << "invalid issue date = " << it->second << " (name = " << featureName << ")\n";
      return false;
    }

    it = featureTree.find("expire"); // конец даты лицензии

    if (featureTree.end() == it) {
      LOG(error) << "expire not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    if (!lickey::Load(featureInfo.expireDate, it->second)) {
      LOG(error) << "invalid expire date = " << it->second << " (name = " << featureName << ")\n";
      return false;
    }

    it = featureTree.find("num"); // количество выданных лицензий

    if (featureTree.end() == it) {
      LOG(error) << "num not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    featureInfo.numLics = boost::lexical_cast<int>(it->second);
    it = featureTree.find("sign"); // контрольная сумма лицензии

    if (featureTree.end() == it) {
      LOG(error) << "sign not found in feature line (name = " << featureName << ")\n";
      return false;
    }

    featureInfo.sign = it->second;
    LOG(info) << "done to convert feature successfully (name = " << featureName << ")\n";
    return true;
  }
}
