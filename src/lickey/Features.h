#ifndef FEATURES_H
#define FEATURES_H
#include "FeatureInfo.h"

namespace lickey {
  class Hash;

  class features : public std::map<std::string, FeatureInfo> {
   public:
    auto is_valid(const std::string &feature_name) const -> bool;
    auto is_expired(const std::string &feature_name) const -> bool;
    auto is_exist(const std::string &feature_name) const -> bool;
  };
}
#endif // FEATURES_H
