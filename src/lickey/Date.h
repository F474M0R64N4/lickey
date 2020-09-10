#ifndef DATE_H
#define DATE_H
#include <boost/date_time/gregorian/gregorian.hpp>

namespace lickey {
  using Date = boost::gregorian::date;

  auto Load(Date &date, const std::string &str) -> bool;
  auto SetToday(Date &date) -> void;
  auto ToString(const Date &date) -> std::string;
}
#endif // DATE_H
