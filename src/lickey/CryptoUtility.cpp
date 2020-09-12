#include "stdafx.h"
#include "CryptoUtility.h"

#include "mbedcrypto/cipher.hpp"
#include "mbedcrypto/hash.hpp"
#include "mbedcrypto/tcodec.hpp"

using namespace mbedcrypto;
using namespace std;

namespace lickey
{
	auto encrypt(const std::string& data, const std::string& key, const std::string& iv, std::string& dest) -> bool
	{
		const auto ciphered_buffer =
			cipher::encrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key + key, data);
		dest = ciphered_buffer;
		return true;
	}

	auto decrypt(const std::string& data, const std::string& key, const std::string& iv, std::string& dest) -> bool
	{
		const auto decrypted_buffer = cipher::decrypt(cipher_t::aes_256_cbc, padding_t::pkcs7, iv, key + key,
		                                              data);
		dest = decrypted_buffer;
		return true;
	}

	auto md5(const std::string& data, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::md5, data);
		return true;
	}

	auto sha256(std::string& data, std::string& hash) -> bool
	{
		hash = make_hash(hash_t::sha256, data);
		return false;
	}

	auto encode_base64(const std::string& data, std::string& str) -> void
	{
		str = to_base64(data);
	}

	auto decode_base64(std::string& str, std::string& data) -> void
	{
		data = base64::decode(str);
	}

	auto make_salt(salt& salt) -> bool
	{
		std::string encoded;

		// TODO: salt
		const std::string tmp = "salt";

		encode_base64(tmp, encoded);
		salt = encoded;
		return true;
	}
}
