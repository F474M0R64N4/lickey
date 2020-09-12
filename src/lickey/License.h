#ifndef LICENSE_H
#define LICENSE_H
#include "Features.h"
#include "HardwareKey.h"
#include "Salt.h"

namespace lickey {
  class license {
    friend class LicenseManager;

    char file_version_{};
    features features_;
    HardwareKey key_;
    Salt explicit_salt_;
    Salt implicit_salt_;
    date last_used_date_;

   public:
    license();
    license(const license &obj);
    virtual ~license();

    auto operator=(const license &obj) -> license&;

    auto feature_map() -> features & {
      return features_;
    }
  };
};
#endif // LICENSE_H
