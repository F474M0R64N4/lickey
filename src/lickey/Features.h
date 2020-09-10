#ifndef FEATURES_H
#define FEATURES_H
#include "FeatureInfo.h"

namespace lickey {
  class Hash;

  class Features : public std::map<std::string, FeatureInfo> {
   public:
    auto IsValid(const std::string &featureName) const -> bool;
    auto IsExpired(const std::string &featureName) const -> bool;
    auto IsExist(const std::string &featureName) const -> bool;
  };
}
#endif // FEATURES_H
