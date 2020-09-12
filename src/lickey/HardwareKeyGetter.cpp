#include "stdafx.h"
#include "HardwareKeyGetter.h"

#include "smbios.hpp"

#include <Windows.h>

#include <mbedcrypto/hash.hpp>
#include <mbedcrypto/tcodec.hpp>

using namespace mbedcrypto;
using namespace smbios;

namespace lickey
{
	hwid_getter::hwid_getter() = default;

	hwid_getter::~hwid_getter() = default;

	auto hwid_getter::operator()() const -> hardware_keys
	{
		hardware_keys keys;

		// Query size of SMBIOS data.
		const DWORD smbios_data_size = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);

		// Allocate memory for SMBIOS data
		auto* const heap_handle = GetProcessHeap();
		auto* const smbios_data = static_cast<raw_smbios_data*>(HeapAlloc(heap_handle, 0,
		                                                                  static_cast<size_t>(smbios_data_size)));
		if (static_cast<bool>(smbios_data))
		{
			// Retrieve the SMBIOS table
			const DWORD bytes_written = GetSystemFirmwareTable('RSMB', 0, smbios_data, smbios_data_size);
			if (bytes_written != smbios_data_size)
			{
				return keys;
			}

			std::string hardware;
			parse(smbios_data_size, heap_handle, smbios_data, hardware);

			auto sha256_value = to_hex(make_hash(hash_t::sha256, hardware));

			hardware_key key;
			key.key_ = move(sha256_value);
			keys.push_back(key);

			return keys;
		}
		return keys;
	}

	inline auto hwid_getter::parse(const DWORD smbios_data_size, void* const heap_handle,
	                               raw_smbios_data* const smbios_data, std::string& hardware) -> void
	{
		// Process the SMBIOS data and free the memory under an exit label
		parser meta;
		auto* const buff = smbios_data->smbios_table_data;
		const auto buff_size = static_cast<size_t>(smbios_data_size);

		meta.feed(buff, buff_size);

		for (auto& header : meta.headers)
		{
			string_array_t strings;
			parser::extract_strings(header, strings);

			switch (header->type_header)
			{
			case types::baseboard_info:
				{
					auto* const x = reinterpret_cast<baseboard_info*>(header);

					if (static_cast<int>(x->length_header) == 0)
					{
						break;
					}

					hardware.append(strings[x->manufacturer_name]);
					hardware.append(strings[x->product_name]);
				}
				break;

			case types::bios_info:
				{
					auto* const x = reinterpret_cast<bios_info*>(header);

					if (static_cast<int>(x->length_header) == 0)
					{
						break;
					}
					hardware.append(strings[x->vendor]);
					hardware.append(strings[x->version]);
				}
				break;

			case types::processor_info:
				{
					auto* const x = reinterpret_cast<proc_info*>(header);

					if (static_cast<int>(x->length_header) == 0)
					{
						break;
					}
					hardware.append(strings[x->manufacturer]);
					hardware.append(strings[x->version]);
					hardware.append(std::to_string(static_cast<long>(x->id)));
				}
				break;

			default:
				break;
			}
		}

		HeapFree(heap_handle, 0, smbios_data);
	}
}
