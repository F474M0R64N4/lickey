#ifndef HARDWAREKEYGETTER_H
#define HARDWAREKEYGETTER_H
#include "HardwareKey.h"
#include "smbios.hpp"

namespace lickey {
  class hwid_getter {
   public:
    hwid_getter();
    virtual ~hwid_getter();

  	auto operator()() const -> hardware_keys;
    static auto parse(DWORD smbios_data_size, void* heap_handle, smbios::raw_smbios_data* smbios_data,
               std::string& hardware) -> void;

  };
}
#endif // HARDWAREKEYGETTER_H
