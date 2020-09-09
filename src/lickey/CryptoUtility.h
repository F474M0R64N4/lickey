#pragma once
#include "Salt.h"

#include <boost/static_string/static_string.hpp>

namespace lickey
{
	auto Encrypt(const boost::static_string<65536>& data, boost::static_string<32> key, const std::string& iv, boost::static_string<65536>& dest,
	             size_t& destlen) -> bool;

	auto Decrypt(const boost::static_string<65536>& data, boost::static_string<32> key, const std::string& iv, boost::static_string<65536>& dest,
	             size_t& destlen) -> bool;

	auto MD5(const boost::static_string<65536>& data, boost::static_string<65536>& hash) -> bool;

	auto SHA256(std::string& data, boost::static_string<65536>& hash) -> bool;

	auto EncodeBase64(boost::static_string<65536>& data, boost::static_string<65536>& str) -> void;

	auto DecodeBase64(std::string& str, boost::static_string<65536>& data, int& datalen) -> void;

	auto MakeSalt(Salt& salt) -> bool;
}
