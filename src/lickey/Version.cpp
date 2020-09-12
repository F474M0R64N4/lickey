#include "stdafx.h"
#include "Version.h"


namespace lickey {
  Version::Version()
    : version_(1) { // first version
  }


  Version &Version::get_instance() {
    static Version obj;
    return obj;
  }


  Version::~Version() = default;
}
