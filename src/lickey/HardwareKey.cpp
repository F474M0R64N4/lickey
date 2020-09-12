#include "stdafx.h"
#include "HardwareKey.h"

namespace lickey {
  HardwareKey::HardwareKey() = default;

  HardwareKey::HardwareKey(const HardwareKey &obj) = default;


  HardwareKey::HardwareKey(const std::string &obj)
    : key(obj) {
  }


  HardwareKey::~HardwareKey() = default;


  HardwareKey &HardwareKey::operator=(const HardwareKey &obj) = default;


  HardwareKey &HardwareKey::operator=(const std::string &obj) {
    key = obj;
    return *this;
  }
}
