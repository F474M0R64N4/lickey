#include "stdafx.h"
#include "CryptoUtility.h"

#include <algorithm>

#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"
#include "mbedcrypto/cipher.hpp"
#include "mbedcrypto/rnd_generator.hpp"

#include <boost/range/algorithm/count.hpp>

using namespace mbedcrypto;
using namespace std;

namespace lickey {

  bool Encrypt(std::string data, const size_t datalen, std::string key, std::string iv,
    std::string &dest, size_t &destlen) {
    const auto ciphered_buffer = cipher::encrypt(cipher_t::aes_128_cbc, padding_t::pkcs7, iv, key, data);
    dest = ciphered_buffer;
    return true;
  }

  bool Decrypt(std::string data, const size_t datalen, std::string key, std::string iv,
    std::string &dest, size_t &destlen) {
    auto decrypted_buffer = cipher::decrypt(cipher_t::aes_128_cbc, padding_t::pkcs7, iv, key, data);
    dest = decrypted_buffer;
    return true;
  }

  bool MD5(
    std::string data,
    size_t datalen,
    std::string &hash) {
    hash = make_hash(hash_t::md5, data);
    datalen = data.length();
    return true;
  }

  bool SHA256(
    std::string &data,
    size_t datalen,
    std::string &hash) {
    hash = make_hash(hash_t::sha256, data);
    return false;
  }

  void EncodeBase64(
    std::string &data,
    std::string &str) {
    str = to_base64(data);
  }

  void DecodeBase64(
    std::string &str,
    std::string &data,
    int &datalen) {
    data = base64::decode(str);
    datalen = str.length();
  }


  bool MakeSalt(Salt &salt) {
    std::string tmp;
    std::string encoded;

    // TODO: заглушка
    if (tmp.empty()) {
      tmp = "salt";
    }

    EncodeBase64(tmp, encoded);
    salt = encoded;
    return true;
  }

}
