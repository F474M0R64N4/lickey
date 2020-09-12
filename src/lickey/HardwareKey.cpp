#include "stdafx.h"
#include "HardwareKey.h"

#include <utility>

namespace lickey {
  HardwareKey::HardwareKey() = default;

  HardwareKey::HardwareKey(const HardwareKey &obj) = default;


  HardwareKey::HardwareKey(std::string obj)
    : key(std::move(obj)) {
  }


  HardwareKey::~HardwareKey() = default;


  HardwareKey &HardwareKey::operator=(const HardwareKey &obj) = default;


  HardwareKey &HardwareKey::operator=(const std::string &obj) {
    key = obj;
    return *this;
  }
}
