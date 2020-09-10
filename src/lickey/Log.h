#ifndef LOG_H
#define LOG_H
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>

namespace lickey {
#define LOG(tag) BOOST_LOG_TRIVIAL(tag)
}
#endif // LOG_H
