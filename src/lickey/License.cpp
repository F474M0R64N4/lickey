#include "stdafx.h"
#include "License.h"
#include "Version.h"


namespace lickey {
  license::license()
    : file_version_(VERSION()) {
  }

  license::license(const license &obj) = default;

  license::~license() = default;

  license &license::operator=(const license &obj) = default;
}
