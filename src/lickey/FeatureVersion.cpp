#include "stdafx.h"
#include "FeatureVersion.h"


namespace lickey {
  feature_version::feature_version() = default;

  feature_version::feature_version(const feature_version &obj) = default;

  feature_version::~feature_version() = default;

  auto feature_version::operator=(const feature_version& obj) -> feature_version& = default;

  auto feature_version::operator=(const std::string& v) -> feature_version&
  {
    version_ = v;
    return *this;
  }
}
