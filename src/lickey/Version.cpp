#include "stdafx.h"
#include "Version.h"

namespace lickey
{
	version::version()
		: version_(1)
	{
		// first version
	}

	auto version::get_instance() -> version&
	{
		static version obj;
		return obj;
	}

	version::~version() = default;
}
