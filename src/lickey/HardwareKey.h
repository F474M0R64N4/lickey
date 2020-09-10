#ifndef HARDWAREKEY_H
#define HARDWAREKEY_H

namespace lickey {
  class HardwareKey final {
    friend class HardwareKeyGetter;

   public:
    HardwareKey();
    HardwareKey(const HardwareKey &obj);
    explicit HardwareKey(const std::string &obj);
    ~HardwareKey();
    auto operator=(const HardwareKey &obj) -> HardwareKey&;
    auto operator=(const std::string &obj) -> HardwareKey&;

    auto Value() const -> std::string {
      return key;
    }

   private:
    std::string key;
  };

  using HardwareKeys = std::vector<HardwareKey>;
}
#endif // HARDWAREKEY_H
