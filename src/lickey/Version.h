#ifndef VERSION_H
#define VERSION_H

namespace lickey {
  class Version {
    char version_;
   public:
    static auto get_instance() -> Version&;
    virtual ~Version();

    auto value() const -> unsigned int {
      return version_;
    }

   private:
    Version();
  };


#define VERSION() Version::get_instance().value()
}
#endif // VERSION_H
