#include "stdafx.h"
#include "CryptoUtility.h"

#include "mbedcrypto/cipher.hpp"
#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"

using namespace mbedcrypto;
using namespace std;

namespace lickey
{
	auto Encrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool
	{
		const auto ciphered_buffer = cipher::encrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key.append(key), data);
		dest = ciphered_buffer;
		return true;
	}

	auto Decrypt(const std::string& data, std::string key, const std::string& iv, std::string& dest) -> bool
	{
		const auto decrypted_buffer = cipher::decrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key.append(key), data);
		dest = decrypted_buffer;
		return true;
	}

	auto MD5(const std::string& data, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::md5, data);
		return true;
	}

	auto SHA256(std::string& data, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::sha256, data);
		return false;
	}

	auto EncodeBase64(const std::string& data, std::string& str) -> void
	{
		str = to_base64(data);
	}

	auto DecodeBase64(std::string& str, std::string& data, int& datalen) -> void
	{
		data = base64::decode(str);
		datalen = static_cast<int>(str.length());
	}

	auto MakeSalt(Salt& salt) -> bool
	{
		std::string encoded;

		// TODO: salt
		const std::string tmp = "salt";

		EncodeBase64(tmp, encoded);
		salt = encoded;
		return true;
	}
}
