#include "stdafx.h"
#include "Hash.h"


namespace lickey {
  Hash::Hash()
    : hash("") {
  }


  Hash::Hash(const Hash &obj) = default;


  Hash::~Hash()  = default;


  Hash &Hash::operator=(const Hash &obj)  = default;


  Hash &Hash::operator=(const std::string &other) {
    hash = other;
    return *this;
  }


  bool Hash::operator==(const Hash &other) const {
    return other.hash == hash;
  }


  bool Hash::operator!=(const Hash &other) const {
    return !(*this == other);
  }
}
