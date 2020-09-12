#include "stdafx.h"
#include "Hash.h"


namespace lickey {
  hash::hash()
    : hash_value_("") {
  }


  hash::hash(const hash &obj) = default;


  hash::~hash()  = default;


  hash &hash::operator=(const hash &obj)  = default;


  hash &hash::operator=(const std::string &other) {
    hash_value_ = other;
    return *this;
  }


  bool hash::operator==(const hash &other) const {
    return other.hash_value_ == hash_value_;
  }


  bool hash::operator!=(const hash &other) const {
    return !(*this == other);
  }
}
