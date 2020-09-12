#include <iostream>

#include "LicenseManager.h"
#include "HardwareKeyGetter.h"
#include "License.h"

int main()
{
	lickey::hwid_getter key_getter;
	auto keys = key_getter();

	lickey::license_manager license_manager("v", "a");
	lickey::license license;
	license_manager.load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\vl(8613cff15aca54d4b41de733b957c9b84377c4cbe95d63f9e5dc3540cdabbce0))",
	                     keys.front(), license);

	if (license.feature_map().is_expired("full") == false)
	{
		std::cout << "full:: the license is not expired\n";
	}
	else
	{
		std::cout << "full:: the license is expired\n";
	}

	if (license.feature_map().is_valid("full") == true)
	{
		std::cout << "full:: license is valid\n";
	}
	else
	{
		std::cout << "full:: license is not valid\n";
	}
	//// trial
	//if (license.FeatureMap().IsExpired("trial") == false)
	//{
	//	std::cout << "trial:: the license is not expired\n";
	//}
	//else
	//{
	//	std::cout << "trial:: the license is expired\n";
	//}

	//if (license.FeatureMap().IsValid("trial") == true)
	//{
	//	std::cout << "trial:: license is valid\n";
	//}
	//else
	//{
	//	std::cout << "trial:: license is not valid\n";
	//}
	//// 2days
	//if (license.FeatureMap().IsExpired("2days") == false)
	//{
	//	std::cout << "2days:: the license is not expired\n";
	//}
	//else
	//{
	//	std::cout << "2days:: the license is expired\n";
	//}

	//if (license.FeatureMap().IsValid("full") == true)
	//{
	//	std::cout << "2days:: license is valid\n";
	//}
	//else
	//{
	//	std::cout << "2days:: license is not valid\n";
	//}


	getchar();
}
