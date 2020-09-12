#include "stdafx.h"
#include "FeatureVersion.h"


namespace lickey {
  FeatureVersion::FeatureVersion() = default;


  FeatureVersion::FeatureVersion(const FeatureVersion &obj)
    : version(obj.version) {
  }


  FeatureVersion::~FeatureVersion() = default;


  FeatureVersion &FeatureVersion::operator=(const FeatureVersion &obj) = default;


  FeatureVersion &FeatureVersion::operator=(const std::string &v) {
    version = v;
    return *this;
  }
}
