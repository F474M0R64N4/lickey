#pragma once
#include <boost/date_time/gregorian/gregorian.hpp>

namespace lickey
{
	using Date = boost::gregorian::date;

	auto Load(Date& date, const std::string& str) -> bool;
	auto SetToday(Date& date) -> void;
	auto ToString(const Date& date) -> std::string;
}
