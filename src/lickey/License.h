#ifndef LICENSE_H
#define LICENSE_H
#include "Features.h"
#include "HardwareKey.h"
#include "Salt.h"

namespace lickey
{
	class license
	{
		friend class license_manager;

		std::string file_version_;
		features features_;
		hardware_key key_;
		salt explicit_salt_;
		salt implicit_salt_;
		date last_used_date_;

	public:
		license();
		license(const license& obj);
		virtual ~license();

		auto operator=(const license& obj) -> license&;

		auto feature_map() -> features&
		{
			return features_;
		}
	};
};
#endif // LICENSE_H
