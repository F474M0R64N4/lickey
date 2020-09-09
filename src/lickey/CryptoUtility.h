#pragma once
#include "Salt.h"

namespace lickey {
  auto Encrypt(
    std::string data, const size_t datalen, std::string key, std::string iv,
    std::string &dest, size_t &destlen) -> bool;

  auto Decrypt(
    std::string data, const size_t datalen, std::string key, std::string iv,
    std::string &dest, size_t &destlen) -> bool;

  auto MD5(
    std::string data,
    size_t datalen,
    std::string &hash) -> bool;

  auto SHA256(
    std::string &data,
    size_t datalen,
    std::string &hash) -> bool;

  auto EncodeBase64(
    std::string &data,
    std::string &str) -> void;

  auto DecodeBase64(
    std::string &str,
    std::string &data,
    int &datalen) -> void;

  auto MakeSalt(Salt &salt) -> bool;
}
