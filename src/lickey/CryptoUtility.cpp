#include "stdafx.h"
#include "CryptoUtility.h"

#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"
#include "mbedcrypto/cipher.hpp"

using namespace mbedcrypto;
using namespace std;

namespace lickey
{
	auto Encrypt(const boost::static_string<65536>& data, boost::static_string<32> key, const std::string& iv, boost::static_string<65536>& dest,
	             size_t& destlen) -> bool
	{
		std::string tmp = key.c_str();
		tmp.append(key.c_str());
		const auto ciphered_buffer = cipher::encrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, tmp, data.c_str());
		dest = ciphered_buffer;
		return true;
	}

	auto Decrypt(const boost::static_string<65536>& data, boost::static_string<32> key, const std::string& iv, boost::static_string<65536>& dest,
	             size_t& destlen) -> bool
	{
		std::string tmp = key.c_str();
		tmp.append(key.c_str());
		const auto decrypted_buffer = cipher::decrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, tmp, data.c_str());
		dest = decrypted_buffer;
		return true;
	}

	auto MD5(const boost::static_string<65536>& data, boost::static_string<65536>& hash) -> bool
	{
		const std::string tmp(data.c_str());
		const std::string tmp_hash = make_hash(hash_t::md5, tmp);
		hash.append(tmp_hash);
		return true;
	}

	auto SHA256(std::string& data, boost::static_string<65536>& hash) -> bool
	{
		hash = make_hash(hash_t::sha256, data);
		return false;
	}

	auto EncodeBase64(boost::static_string<65536>& data, boost::static_string<65536>& str) -> void
	{
		str = to_base64(data.c_str());
	}

	auto DecodeBase64(std::string& str, boost::static_string<65536>& data, int& datalen) -> void
	{
		data = base64::decode(str);
		datalen = static_cast<int>(str.length());
	}

	auto MakeSalt(Salt& salt) -> bool
	{
		boost::static_string<65536> encoded;

		// TODO: salt
		boost::static_string<65536> tmp = "salt";

		EncodeBase64(tmp, encoded);
		salt = encoded.c_str();
		return true;
	}
}
