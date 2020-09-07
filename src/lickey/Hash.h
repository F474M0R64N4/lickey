#pragma once

namespace lickey
{
	class Hash
	{
		std::string hash;

	public:
		Hash();
		Hash(const Hash& obj);
		virtual ~Hash();
		auto operator=(const Hash& obj) -> Hash&;
		auto operator=(const std::string& other) -> Hash&;

		auto operator==(const Hash& other) const -> bool;
		auto operator!=(const Hash& other) const -> bool;

		auto Value() const -> std::string
		{
			return hash;
		}
	};
}
