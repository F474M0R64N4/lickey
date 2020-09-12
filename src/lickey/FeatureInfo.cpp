#include "stdafx.h"
#include "FeatureInfo.h"
#include "Date.h"

namespace lickey
{
	feature_info::feature_info(): num_lics_(0)
	{
	}

	feature_info::~feature_info() = default;

	auto feature_info::is_valid() const -> bool
	{
		if (1 > num_lics_)
		{
			LOG(error) << "the number of license is zero";
			return false;
		}

		if (check_sum_ != sign_)
		{
			LOG(error) << "invalid sign";
			return false;
		}

		// Проверка на переведенное время

		date today;
		set_today(today);

		const int days_after_license_start = (today - issue_date_).days();
		const int days_before_license_end = (expire_date_ - today).days();

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

	auto feature_info::is_expired() const -> bool
	{
		date today;
		set_today(today);
		return today > expire_date_;
	}
}
