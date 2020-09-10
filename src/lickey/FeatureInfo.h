#ifndef FEATUREINFO_H
#define FEATUREINFO_H
#include "FeatureVersion.h"
#include "Date.h"
#include "Hash.h"

namespace lickey {
  class FeatureInfo {
    friend class LicenseManager;

   public:
    FeatureInfo();
    virtual ~FeatureInfo();
  	
    auto IsValid() const -> bool;
    auto IsExpired() const -> bool;

    auto Version() const -> const FeatureVersion & {
      return version;
    }

    auto NumLics() const -> unsigned int {
      return numLics;
    }

    auto IssueDate() const -> const Date & {
      return issueDate;
    }

    auto ExpireDate() const -> const Date & {
      return expireDate;
    }

    auto Sign() const -> const Hash & {
      return sign;
    }

   private:
    FeatureVersion version;
    unsigned int numLics;
    Date issueDate;
    Date expireDate;
    Hash sign; ///< recoded
    Hash checkSum; ///< calculated
  };
}
#endif // FEATUREINFO_H
