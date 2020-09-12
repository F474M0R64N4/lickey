#ifndef FEATUREINFO_H
#define FEATUREINFO_H
#include "Date.h"
#include "FeatureVersion.h"
#include "Hash.h"

namespace lickey {
  class FeatureInfo {
    friend class LicenseManager;

   public:
    FeatureInfo();
    virtual ~FeatureInfo();
  	
    auto is_valid() const -> bool;
    auto is_expired() const -> bool;

    auto version() const -> const feature_version & {
      return version_;
    }

    auto num_lics() const -> unsigned int {
      return num_lics_;
    }

    auto issue_date() const -> const date & {
      return issue_date_;
    }

    auto expire_date() const -> const date & {
      return expire_date_;
    }

    auto sign() const -> const Hash & {
      return sign_;
    }

   private:
    feature_version version_;
    unsigned int num_lics_;
    date issue_date_;
    date expire_date_;
    Hash sign_; ///< recoded
    Hash check_sum_; ///< calculated
  };
}
#endif // FEATUREINFO_H
