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

		bool Load(const std::string& filepath, const HardwareKey& key, License& license);
		bool isLicenseDecrypt(const HardwareKey& key, License& license, int decodedSize2, unsigned char* decoded2);
		bool isLicenseDataSectionRead(const HardwareKey& key, License& license, const std::vector<std::string>& lines);
		bool isLicenseRead(const std::string& filepath, const HardwareKey& key, License& license);

		bool Update();
		bool UpdateLicense();

		bool Save(const std::string& filepath, const HardwareKey& key, License& license);
		static void Add(
			const std::string& featureName,
			const FeatureVersion& featureVersion,
			const Date& issueDate,
			const Date& expireDate,
			unsigned int numLics,
			License& license);

		const std::string& VendorName() const
		{
			return vendorName;
		};

		const std::string& AppName() const
		{
			return appName;
		};

		const std::string& LicenseFilepath() const
		{
			return licenseFilepath;
		};

		bool IsLicenseLoaded() const
		{
			return isLicenseLoaded;
		};

	private:
		static bool ConvertFeature(
			const std::string& line,
			std::string& featureName,
			FeatureInfo& featureInfo);
	};
}
