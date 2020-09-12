#ifndef CRYPTOUTILITY_H
#define CRYPTOUTILITY_H
#include "Salt.h"

namespace lickey
{
	auto Encrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool;

	auto Decrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool;

	auto MD5(const std::string& data, std::string& hash) -> bool;

	auto SHA256(std::string& data, std::string& hash) -> bool;

	auto EncodeBase64(const std::string& data, std::string& str) -> void;

	auto DecodeBase64(std::string& str, std::string& data, int& datalen) -> void;

	auto MakeSalt(Salt& salt) -> bool;
}
#endif // CRYPTOUTILITY_H
