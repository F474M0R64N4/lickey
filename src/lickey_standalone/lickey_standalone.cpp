#include <iostream>

#include "LicenseManager.h"
#include "HardwareKey.h"
#include "HardwareKeyGetter.h"
#include "License.h"

int main()
{
	lickey::HardwareKeyGetter keyGetter;
	lickey::HardwareKeys keys = keyGetter();

	if (keys.empty())
	{
		//return 0;
	}

	lickey::LicenseManager licMgr("v", "a");
	lickey::License lic;
	licMgr.Load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\werds(84-16-F9-F5-79-9F))", keys.front(), lic);

	if (lic.FeatureMap().IsExpired("full") == false)
	{
		std::cout << "the license is not expired\n";
	}
	
	if (lic.FeatureMap().IsValid("full") == true)
	{
		std::cout << "license is valid\n";
	}
}
