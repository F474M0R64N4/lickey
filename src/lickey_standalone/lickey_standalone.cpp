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
		return;
	}

	lickey::LicenseManager licMgr("v", "a");
	lickey::License lic;
	licMgr.Load(R"(C:\Users\WORK\Desktop\lickey\src\lickey_gen\x64\Debug\werds(84-16-F9-F5-79-9F))", keys.front(), lic);

	std::cout << "Hello World!\n";
}
