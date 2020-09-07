#pragma once
#include "Date.h"
#include "License.h"

namespace lickey
{
	class HardwareKey;
	class FeatureInfo;
	class FeatureVersion;

	class LicenseManager
	{
		std::string vendorName;
		std::string appName;

		std::string licenseFilepath;
		bool isLicenseLoaded;
		License loadedLicense;

	public:
		LicenseManager(std::string  vn, std::string  an);
		virtual ~LicenseManager();

		auto Load(const std::string& filepath, const HardwareKey& key, License& license) -> bool;
		auto isLicenseDecrypt(const HardwareKey& key, License& license, int decodedSize2,
		                      unsigned char* decoded2) -> bool;
		auto isLicenseDataSectionRead(const HardwareKey& key, License& license,
		                              const std::vector<std::string>& lines) -> bool;
		auto isLicenseRead(const std::string& filepath, const HardwareKey& key, License& license) -> bool;

		auto Update() -> bool;
		auto UpdateLicense() -> bool;

		auto Save(const std::string& filepath, const HardwareKey& key, License& license) -> bool;
		static auto Add(
			const std::string& featureName,
			const FeatureVersion& featureVersion,
			const Date& issueDate,
			const Date& expireDate,
			unsigned int numLics,
			License& license) -> void;

		auto VendorName() const -> const std::string&
		{
			return vendorName;
		}

		auto AppName() const -> const std::string&
		{
			return appName;
		}

		auto LicenseFilepath() const -> const std::string&
		{
			return licenseFilepath;
		}

		auto IsLicenseLoaded() const -> bool
		{
			return isLicenseLoaded;
		}

	private:
		static auto ConvertFeature(
			const std::string& line,
			std::string& featureName,
			FeatureInfo& featureInfo) -> bool;
	};
}
