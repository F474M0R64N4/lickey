#pragma once

namespace lickey
{
	class Salt
	{
		std::string salt;

	public:
		Salt();
		Salt(const Salt& obj);
		virtual ~Salt();
		auto operator=(const Salt& obj) -> Salt&;
		auto operator=(const std::string& obj) -> Salt&;

		auto Value() const -> std::string
		{
			return salt;
		}
	};
}
