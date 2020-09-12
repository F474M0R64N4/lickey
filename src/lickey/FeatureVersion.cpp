#include "stdafx.h"
#include "FeatureVersion.h"


namespace lickey {
  feature_version::feature_version() = default;


  feature_version::feature_version(const feature_version &obj) = default;


  feature_version::~feature_version() = default;


  feature_version &feature_version::operator=(const feature_version &obj) = default;


  feature_version &feature_version::operator=(const std::string &v) {
    version_ = v;
    return *this;
  }
}
