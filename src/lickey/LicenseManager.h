#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H
#include "Date.h"
#include "License.h"

namespace lickey {
  class HardwareKey;
  class FeatureInfo;
  class feature_version;

  class LicenseManager {
    std::string vendorName;
    std::string appName;
    std::string licenseFilepath;
    bool isLicenseLoaded;
    license loadedLicense;

   public:
    LicenseManager(std::string  vn, std::string  an);
    virtual ~LicenseManager();

    auto Load(const std::string &filepath, const HardwareKey &key, license &license) -> bool;
    auto isLicenseDecrypt(const HardwareKey &key, license &license, int decoded_size, std::string &decoded) -> bool;
    auto isLicenseDataSectionRead(const HardwareKey &key, license &license, const std::vector<std::string> &lines) -> bool;
    auto isLicenseRead(const std::string &filepath, const HardwareKey &key, license &license) -> bool;

    auto Update() -> bool;
    auto UpdateLicense() -> bool;

    auto Save(const std::string &filepath, const HardwareKey &key, license &license) -> bool;
    static auto Add(
      const std::string &featureName,
      const feature_version &featureVersion,
      const date &issueDate,
      const date &expireDate,
      unsigned int numLics,
      license &license) -> void;

    auto VendorName() const -> const std::string & {
      return vendorName;
    }

    auto AppName() const -> const std::string & {
      return appName;
    }

    auto LicenseFilepath() const -> const std::string & {
      return licenseFilepath;
    }

    auto IsLicenseLoaded() const -> bool {
      return isLicenseLoaded;
    }

   private:
    static auto ConvertFeature(
      const std::string &line,
      std::string &featureName,
      FeatureInfo &featureInfo) -> bool;
  };
}
#endif // LICENSEMANAGER_H
