#include <iostream>

#include "LicenseManager.h"
#include "HardwareKeyGetter.h"
#include "License.h"

int main()
{
	lickey::HardwareKeyGetter key_getter;
	auto keys = key_getter();

	lickey::LicenseManager license_manager("v", "a");
	lickey::License license;
	license_manager.Load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\werds(84-16-F9-F5-79-9F))", keys.front(), license);

	if (license.FeatureMap().IsExpired("full") == false)
	{
		std::cout << "the license is not expired\n";
	}
	
	if (license.FeatureMap().IsValid("full") == true)
	{
		std::cout << "license is valid\n";
	}
}
