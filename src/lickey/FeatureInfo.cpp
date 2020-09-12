#include "stdafx.h"
#include "FeatureInfo.h"
#include "Date.h"


namespace lickey {
  FeatureInfo::FeatureInfo(): numLics(0) {
  }


  FeatureInfo::~FeatureInfo() = default;


  bool FeatureInfo::IsValid() const {
    if (1 > numLics) {
      LOG(error) << "the number of license is zero";
      return false;
    }

    if (checkSum != sign) {
      LOG(error) << "invalid sign";
      return false;
    }

  	// Проверка на переведенное время

	Date today;
    SetToday(today);

  	const int days_after_license_start = (today - issueDate).days();
	const int days_before_license_end = (expireDate - today).days();

    if (days_after_license_start < 0)
	{
		return false;
	}

	if (days_before_license_end < 0)
	{
		return false;
	}
  	
    return true;
  }


  bool FeatureInfo::IsExpired() const {
    Date today;
    SetToday(today);
    return today > expireDate;
  }
}
