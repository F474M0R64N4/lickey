#pragma once
#include "Salt.h"

namespace lickey
{
	auto Encrypt(const std::string& data, size_t datalen, std::string key, const std::string& iv, std::string& dest,
	             size_t& destlen) -> bool;

	auto Decrypt(const std::string& data, size_t datalen, std::string key, const std::string& iv, std::string& dest,
	             size_t& destlen) -> bool;

	auto MD5(const std::string& data, /*size_t datalen,*/ std::string& hash) -> bool;

	auto SHA256(std::string& data, size_t datalen, std::string& hash) -> bool;

	auto EncodeBase64(std::string& data, std::string& str) -> void;

	auto DecodeBase64(std::string& str, std::string& data, int& datalen) -> void;

	auto MakeSalt(Salt& salt) -> bool;
}
