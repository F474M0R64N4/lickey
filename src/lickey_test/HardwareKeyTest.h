#pragma once
#define BOOST_TEST_MODULE my_mod
#include <HardwareKey.h>
#include <HardwareKeyGetter.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(HardwareKey)

BOOST_AUTO_TEST_CASE(Constructor01)
{
    lickey::HardwareKeyGetter keyGetter;
    lickey::HardwareKeys keys = keyGetter();

    for(size_t i = 0; i < keys.size(); ++i)
    {
        BOOST_TEST_MESSAGE(keys[i].Value());
    }

    BOOST_CHECK_EQUAL(5, keys.size());
    if(5 == keys.size())
    {
        BOOST_CHECK_EQUAL("84-16-F9-F5-79-9F", keys[0].Value());
    }
}

BOOST_AUTO_TEST_SUITE_END()
