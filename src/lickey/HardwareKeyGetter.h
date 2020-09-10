#ifndef HARDWAREKEYGETTER_H
#define HARDWAREKEYGETTER_H
#include "HardwareKey.h"

namespace lickey {
  class HardwareKeyGetter {
   public:
    HardwareKeyGetter();
    virtual ~HardwareKeyGetter();

    auto operator()() const -> HardwareKeys;
  };
}
#endif // HARDWAREKEYGETTER_H
