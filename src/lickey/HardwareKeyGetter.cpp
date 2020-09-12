#include "stdafx.h"
#include "HardwareKeyGetter.h"

#include "smbios.hpp"

#include <Windows.h>

#include <mbedcrypto/hash.hpp>
#include <mbedcrypto/tcodec.hpp>

using namespace mbedcrypto;
using namespace smbios;

namespace lickey {
  HardwareKeyGetter::HardwareKeyGetter() {
  }


  HardwareKeyGetter::~HardwareKeyGetter() {
  }


  HardwareKeys HardwareKeyGetter::operator()() const {
	  HardwareKeys keys;

	  // Query size of SMBIOS data.
	  const DWORD smbios_data_size = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);

	  // Allocate memory for SMBIOS data
	  auto* const heap_handle = GetProcessHeap();
	  auto* const smbios_data = static_cast<raw_smbios_data*>(HeapAlloc(heap_handle, 0,
		  static_cast<size_t>(smbios_data_size)));
	  if (!smbios_data)
	  {
		  return keys;
	  }

	  // Retrieve the SMBIOS table
	  const DWORD bytes_written = GetSystemFirmwareTable('RSMB', 0, smbios_data, smbios_data_size);
	  if (bytes_written != smbios_data_size)
	  {
		  return keys;
	  }

	  // Process the SMBIOS data and free the memory under an exit label
	  parser meta;
	  auto* const buff = smbios_data->smbios_table_data;
	  const auto buff_size = static_cast<size_t>(smbios_data_size);

	  meta.feed(buff, buff_size);

	  std::string hardware;

	  for (auto& header : meta.headers)
	  {
		  string_array_t strings;
		  parser::extract_strings(header, strings);

		  switch (header->type)
		  {
		  case types::baseboard_info:
		  {
			  auto* const x = reinterpret_cast<baseboard_info*>(header);

			  if (x->length == 0)
				  break;

			  hardware.append(strings[x->manufacturer_name]);
			  hardware.append(strings[x->product_name]);
		  }
		  break;

		  case types::bios_info:
		  {
			  auto* const x = reinterpret_cast<bios_info*>(header);

			  if (x->length == 0)
				  break;
			  hardware.append(strings[x->vendor]);
			  hardware.append(strings[x->version]);
		  }
		  break;

		  case types::processor_info:
		  {
			  auto* const x = reinterpret_cast<proc_info*>(header);

			  if (x->length == 0)
				  break;
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

	  std::string sha256_value = to_hex(make_hash(hash_t::sha256, hardware));

	  HardwareKey key;
	  key.key = move(sha256_value);
	  keys.push_back(key);

	  return keys;

  }
}
