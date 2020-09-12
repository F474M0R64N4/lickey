#ifndef HASH_H
#define HASH_H

namespace lickey
{
	class hash
	{
		std::string hash_value_;

	public:
		hash();
		hash(const hash& obj);
		virtual ~hash();
		auto operator=(const hash& obj) -> hash&;
		auto operator=(const std::string& other) -> hash&;

		auto operator==(const hash& other) const -> bool;
		auto operator!=(const hash& other) const -> bool;

		auto value() const -> std::string
		{
			return hash_value_;
		}
	};
}
#endif // HASH_H
