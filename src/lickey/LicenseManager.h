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
        LicenseManager(const std::string& vn, const std::string& an);
        virtual ~LicenseManager();

        bool Load(const std::string& filepath, const HardwareKey& key, License& license);
        bool Update();
        bool Save(const std::string& filepath, const HardwareKey& key, License& license);
        void Add(
            const std::string& featureName,
            const FeatureVersion& featureVersion,
            const Date& issueDate,
            const Date& expireDate,
            unsigned int numLics,
            License& license);

        const std::string& VenderName() const
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
        bool ConvertFeature(
            const std::string& line,
            std::string& featureName,
            FeatureInfo& featureInfo);
    };

}
