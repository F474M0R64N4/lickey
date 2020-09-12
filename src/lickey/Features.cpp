#include "stdafx.h"
#include "Features.h"


namespace lickey {
  bool features::is_valid(const std::string &feature_name) const {
    const auto cit = (*this).find(feature_name);

    if (end() == cit) {
      LOG(error) << feature_name << " to be checked validity not exist";
      return false;
    }

    return cit->second.is_valid();
  }


  bool features::is_expired(const std::string &feature_name) const {
    const auto cit = (*this).find(feature_name);

    if (end() == cit) {
      LOG(error) << feature_name << " to be checked expire date not exist";
      return false;
    }

    return cit->second.is_expired();
  }


  bool features::is_exist(const std::string &feature_name) const {
    const auto cit = (*this).find(feature_name);
    return end() != cit;
  }
}
