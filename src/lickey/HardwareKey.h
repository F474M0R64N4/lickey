#ifndef HARDWAREKEY_H
#define HARDWAREKEY_H

namespace lickey
{
	class hardware_key final
	{
		friend class hwid_getter;

	public:
		hardware_key();
		hardware_key(const hardware_key& obj);
		explicit hardware_key(std::string obj);
		~hardware_key();
		auto operator=(const hardware_key& obj) -> hardware_key&;
		auto operator=(const std::string& obj) -> hardware_key&;

		auto value() const -> std::string
		{
			return key_;
		}

	private:
		std::string key_;
	};

	using hardware_keys = std::vector<hardware_key>;
}
#endif // HARDWAREKEY_H
