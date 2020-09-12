#include "stdafx.h"
#include "Salt.h"


namespace lickey {
  Salt::Salt() = default;


  Salt::Salt(const Salt &obj) = default;


  Salt::~Salt() = default;

  Salt &Salt::operator=(const Salt &obj) = default;


  Salt &Salt::operator=(const std::string &obj) {
    salt = obj;
    return *this;
  }
}
