#include <algorithm>
#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <FeatureInfo.h>
#include <FileUtility.h>
#include <Date.h>
#include <Log.h>
#include <License.h>
#include <LicenseManager.h>
#include <Version.h>


using namespace lickey;

namespace
{
	auto to_lower_and_trim(std::string& str) -> void
	{
		std::transform(str.begin(), str.end(), str.begin(), tolower);
		boost::trim(str);
	}


	auto add_feature(
		const std::string& feature_name,
		const std::string& feature_version,
		const date& issue,
		const std::string& expire_date,
		const unsigned int num_lics,
		license& lic) -> bool
	{
		date expire;

		if (load(expire, expire_date))
		{
			feature_version version;
			version = feature_version;
			LicenseManager::Add(feature_name, version, issue, expire, num_lics, lic);
			std::cout << "done to add feature = " << feature_name << "\n";
			std::cout << "  version = " << feature_version << "\n";
			std::cout << "  issue date = " << to_string(issue) << "\n";
			std::cout << "  expire date = " << expire_date << "\n";
			std::cout << "  num licenses = " << num_lics << "\n";
			return true;
		}

		std::cout << "invalid expire date = " << expire_date << "\n";
		return false;
	}
}


auto main(int argc, char* argv[]) -> int
{
	std::cout << "License generator V" << VERSION() << "\n";
	std::cout << "(half-width characters only / without space and tabspace)\n";
	std::cout << "\n";
	std::string vendor_name;
	std::cout << "vender name:";
	std::cin >> vendor_name;
	boost::trim(vendor_name);
	std::string app_name;
	std::cout << "application name:";
	std::cin >> app_name;
	boost::trim(app_name);
	std::string hardware_key;
	std::cout << "hardware key(sha256 format):";
	std::cin >> hardware_key;
	boost::trim(hardware_key);
	LicenseManager lic_mgr(vendor_name, app_name);
	license lic;

	do
	{
		std::cout << "feature name (\"quit\" to quit this operation):";
		std::string feature;
		std::cin >> feature;
		to_lower_and_trim(feature);

		if ("quit" == feature)
		{
			break;
		}

		std::string feature_version;
		std::cout << "feature version(positive integer):";
		std::cin >> feature_version;
		date issue;
		set_today(issue);
		std::string expire_date;

		do
		{
			std::cout << "expire date(YYYYMMDD format):";
			std::cin >> expire_date;
			to_lower_and_trim(expire_date);

			if ("quit" == expire_date)
			{
				break;
			}

			date tmp;

			if (load(tmp, expire_date))
			{
				break;
			}

			std::cout << "invalid date format\n";
		}
		while (true);

		unsigned int num_lics = 0;

		do
		{
			std::cout << "num licenses(position integer):";
			std::cin >> num_lics;

			if (0 == num_lics)
			{
				std::cout << "num licenses must be more than 0\n";
				continue;
			}

			break;
		}
		while (true);

		if (add_feature(feature, feature_version, issue, expire_date, num_lics, lic))
		{
			continue;
		}

		std::cout << "fail to add new feature\n";
	}
	while (true);

	if (lic.feature_map().empty())
	{
		std::cout << "no feature defined\n";
		std::string buf;
		std::cin >> buf;
		return 0;
	}

	do
	{
		std::string filepath;
		std::cout << "license file name:";
		std::cin >> filepath;
		to_lower_and_trim(filepath);

		if ("quit" == filepath)
		{
			std::cout << "done without saving license file\n";
			break;
		}

		auto base_filepath = GetBaseFilePath(filepath);
		auto extension = GetExtension(filepath);
		std::stringstream filepath_impl;
		filepath_impl << base_filepath << "(" << hardware_key << ")" << extension;

		if (lic_mgr.Save(filepath_impl.str(), HardwareKey(hardware_key), lic))
		{
			std::cout << "done to save into = " << filepath_impl.str() << "\n";
			break;
		}

		std::cout << "fail to save into = " << filepath_impl.str() << "\n";
	}
	while (true);

	std::cout << "please press any key\n";
	std::string buf;
	std::cin >> buf;
	return 0;
}
