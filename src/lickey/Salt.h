#ifndef SALT_H
#define SALT_H

namespace lickey {
  class salt {
    std::string salt_;

   public:
    salt();
    salt(const salt &obj);
    virtual ~salt();
    auto operator=(const salt &obj) -> salt&;
    auto operator=(const std::string &obj) -> salt&;

    auto value() const -> std::string {
      return salt_;
    }
  };
}
#endif // SALT_H
