#ifndef CRYPTOUTILITY_H
#define CRYPTOUTILITY_H
#include "Salt.h"

namespace lickey
{
	auto encrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool;

	auto decrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool;

	auto md5(const std::string& data, std::string& hash) -> bool;

	auto sha256(std::string& data, std::string& hash) -> bool;

	auto encode_base64(const std::string& data, std::string& str) -> void;

	auto decode_base64(std::string& str, std::string& data, int& datalen) -> void;

	auto make_salt(Salt& salt) -> bool;
}
#endif // CRYPTOUTILITY_H
