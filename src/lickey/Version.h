#ifndef VERSION_H
#define VERSION_H

namespace lickey {
  class version {
    char version_;
   public:
    static auto get_instance() -> version&;
    virtual ~version();

    auto value() const -> unsigned int {
      return version_;
    }

    version();
  };


#define LICENSE_VERSION() version::get_instance().value()
}
#endif // VERSION_H
