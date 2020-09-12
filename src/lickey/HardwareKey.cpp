#include "stdafx.h"
#include "HardwareKey.h"

#include <utility>

namespace lickey {
  hardware_key::hardware_key() = default;

  hardware_key::hardware_key(const hardware_key &obj) = default;

  hardware_key::hardware_key(std::string obj)
    : key_(std::move(obj)) {
  }

  hardware_key::~hardware_key() = default;

  auto hardware_key::operator=(const hardware_key& obj) -> hardware_key& = default;

  auto hardware_key::operator=(const std::string& obj) -> hardware_key&
  {
    key_ = obj;
    return *this;
  }
}
