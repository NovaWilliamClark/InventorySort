#pragma once
#include <string>
#include <raylib-cpp.hpp>

enum class EItemType
{
	ARMOR,
	CONSUMABLE,
	UTILITY,
	WEAPON,
};

class Item final
{
public:
	explicit Item(std::string t_name, EItemType t_type, float t_price, int t_quantity);
	Item() = default;
	~Item() = default;

	// Mutator methods
	void setName(const std::string& t_name);
	void setType(EItemType t_type);
	void setPrice(float t_price);
	void setQuantity(int t_quantity);

	// Accessor methods
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] EItemType getType() const;
	[[nodiscard]] float getPrice() const;
	[[nodiscard]] int getQuantity() const;
	[[nodiscard]] std::string getMemoryAddress() const;

private:
	std::string m_name;
	EItemType m_type;
	float m_price{};
	int m_quantity{};
};
