#ifndef LICENSEMANAGERTEST_H
#define LICENSEMANAGERTEST_H
#define BOOST_TEST_MODULE my_mod
#include <LicenseManager.h>
#include <Date.h>
#include <Features.h>
#include <License.h>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(LicenseManagerTest)

BOOST_AUTO_TEST_CASE(Constructor01) {
  lickey::HardwareKeyGetter keyGetter;
  lickey::HardwareKeys keys = keyGetter();

  if (keys.empty()) {
    return;
  }

  {
    lickey::LicenseManager licMgr("v", "a");
    lickey::License lic;
    licMgr.Load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\vl(8613cff15aca54d4b41de733b957c9b84377c4cbe95d63f9e5dc3540cdabbce0))", keys.front(), lic);
    BOOST_CHECK_EQUAL(false, lic.FeatureMap().IsExpired("full"));
    BOOST_CHECK_EQUAL(true, lic.FeatureMap().IsValid("full"));
  }
}

BOOST_AUTO_TEST_SUITE_END()
#endif // LICENSEMANAGERTEST_H
