#ifndef LICENSEMANAGERTEST_H
#define LICENSEMANAGERTEST_H
#define BOOST_TEST_MODULE my_mod
#include <LicenseManager.h>
#include <Date.h>
#include <Features.h>
#include <License.h>
#include <boost/test/included/unit_test.hpp>

#include "HardwareKeyGetter.h"

BOOST_AUTO_TEST_SUITE(license_manager_test)

	BOOST_AUTO_TEST_CASE(Constructor01)
	{
		lickey::HardwareKeyGetter key_getter;
		auto keys = key_getter();

		if (keys.empty())
		{
			return;
		}

		{
			lickey::LicenseManager lic_mgr("v", "a");
			lickey::license lic;
			lic_mgr.Load(
				R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\vl(8613cff15aca54d4b41de733b957c9b84377c4cbe95d63f9e5dc3540cdabbce0))",
				keys.front(), lic);
			BOOST_CHECK_EQUAL(false, lic.feature_map().is_expired("full"));
			BOOST_CHECK_EQUAL(true, lic.feature_map().is_valid("full"));
		}
	}

BOOST_AUTO_TEST_SUITE_END()
#endif // LICENSEMANAGERTEST_H
