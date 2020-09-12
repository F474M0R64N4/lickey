#ifndef HARDWAREKEYTEST_H
#define HARDWAREKEYTEST_H
#define BOOST_TEST_MODULE my_mod
#include <HardwareKey.h>
#include <HardwareKeyGetter.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(HardwareKey)

BOOST_AUTO_TEST_CASE(Constructor01)
{
	const lickey::HardwareKeyGetter keyGetter;
	auto keys = keyGetter();

    for (auto& key : keys)
    {
        BOOST_TEST_MESSAGE(key.Value());
    }

    BOOST_CHECK_EQUAL(5, keys.size());
    if(5 == keys.size())
    {
        BOOST_CHECK_EQUAL("8613cff15aca54d4b41de733b957c9b84377c4cbe95d63f9e5dc3540cdabbce0", keys[0].Value());
    }
}

BOOST_AUTO_TEST_SUITE_END()
#endif // HARDWAREKEYTEST_H
