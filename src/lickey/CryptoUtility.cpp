#include "stdafx.h"
#include "CryptoUtility.h"
#include <openssl/evp.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/buffer.h>
#include <algorithm>

#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"
#include "mbedcrypto/cipher.hpp"
#include "mbedcrypto/rnd_generator.hpp"

// to remove warning C4996 for fopen() in openssl/applink.c
#ifdef _WIN32
  #ifdef WIN32
    #define WIN32_PREDEFINED
  #else
    #define WIN32
  #endif
  #pragma warning(push)
  #pragma warning(disable: 4996)
#endif
//#include <openssl/applink.c>
#ifdef WIN32
  #ifdef WIN32_PREDEFINED
    #undef WIN32_PREDEFINED
  #else
    #undef WIN32
  #endif
  #pragma warning(pop)
#endif

using namespace mbedcrypto;
using namespace std;

namespace lickey {

  bool Encrypt_(std::string data, const size_t datalen, std::string key, std::string iv,
    std::string dest, size_t &destlen) {
    const auto ciphered_buffer = cipher::encrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key, data);
    dest = ciphered_buffer;
    return true;
  }

  bool Decrypt_(std::string data, const size_t datalen, std::string key, std::string iv,
    std::string dest, size_t &destlen) {
    auto decrypted_buffer = cipher::decrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key, data);
    dest = decrypted_buffer;
    return true;
  }

  bool MD5_(
    std::string data,
    size_t datalen,
    std::string &hash) {
    hash = make_hash(hash_t::md5, data);
    return true;
  }

  bool SHA256_(
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

  void DecodeBase64_(
    std::string &str,
    std::string &data,
    int &datalen) {
    data = base64::decode(str);
  }


  bool MakeSalt(Salt &salt) {
    std::string tmp;
    std::string encoded;
    EncodeBase64(tmp, encoded);
    salt = encoded;
    return true;
  }

}
