#ifndef FEATUREVERSION_H
#define FEATUREVERSION_H

namespace lickey {
  class feature_version {
    friend class LicenseManager;

   public:
    feature_version();
    feature_version(const feature_version &obj);
    virtual ~feature_version();

    auto operator=(const feature_version &obj) -> feature_version&;
    auto operator=(const std::string &v) -> feature_version&;

    auto value() const -> const std::string & {
      return version_;
    }

   private:
    std::string version_;
  };
}
#endif // FEATUREVERSION_H
