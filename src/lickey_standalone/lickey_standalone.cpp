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
	license_manager.Load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\putin(84-16-F9-F5-79-9F))",
	                     keys.front(), license);

	if (license.FeatureMap().IsExpired("full") == false)
	{
		std::cout << "full:: the license is not expired\n";
	}
	else
	{
		std::cout << "full:: the license is expired\n";
	}

	if (license.FeatureMap().IsValid("full") == true)
	{
		std::cout << "full:: license is valid\n";
	}
	else
	{
		std::cout << "full:: license is not valid\n";
	}
	// trial
	if (license.FeatureMap().IsExpired("trial") == false)
	{
		std::cout << "trial:: the license is not expired\n";
	}
	else
	{
		std::cout << "trial:: the license is expired\n";
	}

	if (license.FeatureMap().IsValid("trial") == true)
	{
		std::cout << "trial:: license is valid\n";
	}
	else
	{
		std::cout << "trial:: license is not valid\n";
	}
	// 2days
	if (license.FeatureMap().IsExpired("2days") == false)
	{
		std::cout << "2days:: the license is not expired\n";
	}
	else
	{
		std::cout << "2days:: the license is expired\n";
	}

	if (license.FeatureMap().IsValid("full") == true)
	{
		std::cout << "2days:: license is valid\n";
	}
	else
	{
		std::cout << "2days:: license is not valid\n";
	}


	getchar();
}
