#pragma once
#include <boost/date_time/gregorian/gregorian.hpp>

namespace lickey
{
	using Date = boost::gregorian::date;

	bool Load(Date& date, const std::string& str);
	void SetToday(Date& date);
	std::string ToString(const Date& date);
}
