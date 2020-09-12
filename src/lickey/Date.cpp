#include "stdafx.h"
#include "Date.h"

namespace lickey
{
	auto load(date& date, const std::string& str) -> bool
	{
		try
		{
			date = boost::gregorian::from_undelimited_string(str);
			return true;
		}
		catch (const std::exception& ex)
		{
			LOG(error) << "invalid date = " << str;
			LOG(error) << "reason = " << ex.what();
			return false;
		}
	}


	auto set_today(date& date) -> void
	{
		date = boost::gregorian::day_clock::local_day();
	}


	auto to_string(const date& date) -> std::string
	{
		return to_iso_string(date);
	}
}
