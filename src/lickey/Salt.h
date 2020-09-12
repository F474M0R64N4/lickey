#ifndef SALT_H
#define SALT_H

namespace lickey {
  class Salt {
    std::string salt_;

   public:
    Salt();
    Salt(const Salt &obj);
    virtual ~Salt();
    auto operator=(const Salt &obj) -> Salt&;
    auto operator=(const std::string &obj) -> Salt&;

    auto value() const -> std::string {
      return salt_;
    }
  };
}
#endif // SALT_H
