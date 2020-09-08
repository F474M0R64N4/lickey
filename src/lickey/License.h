#pragma once
#include "Features.h"
#include "HardwareKey.h"
#include "Salt.h"

namespace lickey {
  class License {
    friend class LicenseManager;

    char fileVersion;
    Features features;
    HardwareKey key;
    Salt explicitSalt;
    Salt implicitSalt;
    Date lastUsedDate;

   public:
    License();
    License(const License &obj);
    virtual ~License();

    auto operator=(const License &obj) -> License&;

    auto FeatureMap() -> Features & {
      return features;
    }
  };
};
