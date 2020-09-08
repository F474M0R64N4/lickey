#pragma once

namespace lickey {
  class Version {
    char version;
   public:
    static auto GetInstance() -> Version&;
    virtual ~Version();

    auto Value() const -> unsigned int {
      return version;
    }

   private:
    Version();
  };


#define VERSION() Version::GetInstance().Value()
}
