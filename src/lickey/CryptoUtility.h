#pragma once
#include "Salt.h"

namespace lickey {
  void InitializeOpenSSL();

  auto Encrypt(
    const char *data,
    size_t datalen,
    const unsigned char *key,
    const unsigned char *iv,
    unsigned char *dest,
    size_t &destlen) -> bool;

  auto Encrypt_(
    const char *data,
    size_t datalen,
    const unsigned char *key,
    const unsigned char *iv,
    unsigned char *dest,
    size_t &destlen) -> bool;

  auto Decrypt(
    const unsigned char *data,
    size_t datalen,
    const unsigned char *key,
    const unsigned char *iv,
    unsigned char *dest,
    size_t &destlen) -> bool;


  auto MD5(
    const char *data,
    size_t datalen,
    unsigned char hash[16]) -> bool;


  auto SHA256(
    const char *data,
    size_t datalen,
    unsigned char hash[32]) -> bool;


  auto EncodeBase64(
    const unsigned char *data,
    int datalen,
    std::string &str) -> void;


  auto EncodeBase64(
    const std::string &data,
    std::string &str) -> void;


  auto DecodeBase64(
    const std::string &str,
    unsigned char *&data,
    int &datalen) -> void;


  auto MakeSalt(Salt &salt) -> bool;
}
