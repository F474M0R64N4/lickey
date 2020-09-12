#include "stdafx.h"
#include "License.h"
#include "Version.h"

namespace lickey
{
	license::license()
		: file_version_(LICENSE_VERSION())
	{
	}

	license::license(const license& obj) = default;

	license::~license() = default;

	auto license::operator=(const license& obj) -> license& = default;
}
