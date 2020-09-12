#include "stdafx.h"
#include "License.h"
#include "Version.h"


namespace lickey {
  License::License()
    : fileVersion(VERSION()) {
  }

  License::License(const License &obj) = default;

  License::~License() = default;

  License &License::operator=(const License &obj) = default;
}
