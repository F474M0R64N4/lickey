#ifndef HARDWAREKEYGETTER_H
#define HARDWAREKEYGETTER_H
#include "HardwareKey.h"
#include "smbios.hpp"

namespace lickey {
  class HardwareKeyGetter {
   public:
    HardwareKeyGetter();
    virtual ~HardwareKeyGetter();

  	auto operator()() const -> HardwareKeys;
    auto parse(DWORD smbios_data_size, void* heap_handle, smbios::raw_smbios_data* smbios_data,
               std::string& hardware) const -> void;

  };
}
#endif // HARDWAREKEYGETTER_H
