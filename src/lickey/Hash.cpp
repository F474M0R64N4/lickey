#include "stdafx.h"
#include "Hash.h"

namespace lickey
{
	hash::hash()
		: hash_value_("")
	{
	}

	hash::hash(const hash& obj) = default;

	hash::~hash() = default;

	auto hash::operator=(const hash& obj) -> hash& = default;

	auto hash::operator=(const std::string& other) -> hash&
	{
		hash_value_ = other;
		return *this;
	}

	auto hash::operator==(const hash& other) const -> bool
	{
		return other.hash_value_ == hash_value_;
	}

	auto hash::operator!=(const hash& other) const -> bool
	{
		return !(*this == other);
	}
}
