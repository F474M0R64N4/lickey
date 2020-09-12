#include "stdafx.h"
#include "Salt.h"

namespace lickey
{
	salt::salt() = default;

	salt::salt(const salt& obj) = default;

	salt::~salt() = default;

	auto salt::operator=(const salt& obj) -> salt& = default;

	auto salt::operator=(const std::string& obj) -> salt&
	{
		salt_ = obj;
		return *this;
	}
}
