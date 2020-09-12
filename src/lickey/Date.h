#ifndef DATE_H
#define DATE_H
#include <boost/date_time/gregorian/gregorian.hpp>

namespace lickey
{
	using date = boost::gregorian::date;

	auto load(date& date, const std::string& str) -> bool;
	auto set_today(date& date) -> void;
	auto to_string(const date& date) -> std::string;
}
#endif // DATE_H
