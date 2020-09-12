#ifndef DATETEST_H
#define DATETEST_H
#define BOOST_TEST_MODULE my_mod
#include <Date.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Date)

BOOST_AUTO_TEST_CASE(Constructor01)
{
	const lickey::date date(2017, 2, 28);

    BOOST_CHECK_EQUAL(2017, date.year());
    BOOST_CHECK_EQUAL(2, date.month());
    BOOST_CHECK_EQUAL(28, date.day());
}

BOOST_AUTO_TEST_CASE(Constructor02)
{
    lickey::date date;
    lickey::load(date, "20170228");
    BOOST_CHECK_EQUAL("20170228", lickey::to_string(date));
}

BOOST_AUTO_TEST_CASE(EQ01)
{
	const lickey::date date1(2017, 2, 28);
    lickey::date date2;
    lickey::load(date2, "20170228");
    BOOST_CHECK_EQUAL(true, date1 == date2);
}

BOOST_AUTO_TEST_SUITE_END()
#endif // DATETEST_H
