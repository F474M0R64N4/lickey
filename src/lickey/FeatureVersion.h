#pragma once

namespace lickey
{
	class FeatureVersion
	{
		friend class LicenseManager;

	public:
		FeatureVersion();
		FeatureVersion(const FeatureVersion& obj);
		virtual ~FeatureVersion();

		auto operator=(const FeatureVersion& obj) -> FeatureVersion&;
		auto operator=(const std::string& v) -> FeatureVersion&;

		auto Value() const -> const std::string&
		{
			return version;
		}

	private:
		std::string version;
	};
}
