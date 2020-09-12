#ifndef LICENSEMANAGER_H
#define LICENSEMANAGER_H
#include "Date.h"
#include "License.h"

namespace lickey
{
	class hardware_key;
	class feature_info;
	class feature_version;

	class license_manager
	{
		std::string vendor_name_;
		std::string app_name_;
		std::string license_filepath_;
		bool is_license_loaded_;
		license loaded_license_;

	public:
		license_manager(std::string vn, std::string an);
		virtual ~license_manager();

		auto load(const std::string& filepath, const hardware_key& key, license& license) -> bool;
		auto is_license_decrypt(const hardware_key& key, license& license,
		                        std::string& decoded) -> bool;
		auto is_license_data_section_read(const hardware_key& key, license& license,
		                                  const std::vector<std::string>& lines) -> bool;
		auto is_license_read(const std::string& filepath, const hardware_key& key, license& license) -> bool;

		auto update() -> bool;
		auto update_license() -> bool;

		auto save(const std::string& filepath, const hardware_key& key, license& license) -> bool;
		static auto add(const std::string& feature_name, const feature_version& feature_version, const date& issue_date,
		                const date& expire_date, unsigned int num_lics, license& license) -> void;

		auto vendor_name() const -> const std::string&
		{
			return vendor_name_;
		}

		auto app_name() const -> const std::string&
		{
			return app_name_;
		}

		auto license_filepath() const -> const std::string&
		{
			return license_filepath_;
		}

		auto is_license_loaded() const -> bool
		{
			return is_license_loaded_;
		}

	private:
		static auto convert_feature(const std::string& line, std::string& feature_name,
		                           feature_info& feature_info) -> bool;
	};
}
#endif // LICENSEMANAGER_H
