#include "InventorySystem.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// InventorySystem constructor
InventorySystem::InventorySystem()
	: m_head(nullptr), m_tail(nullptr), m_size(0)
{}

// Node constructor
InventorySystem::Node::Node(const Item& t_newItem, Node* t_next, Node* t_prev)
	: itemData(t_newItem), next(t_next), prev(t_prev)
{}

// InventorySystem destructor
InventorySystem::~InventorySystem()
{
	// Iterate through the linked list and delete each node
	const Node* current = m_head;
	while (current != nullptr)
	{
		const Node* temp = current;
		current = current->next;
		delete temp;
	}
}

// Check if an item with the given name exists in the inventory
bool InventorySystem::itemExists(const std::string& t_name) const
{
	// Iterate through the linked list and compare each node value to the given name
	const Node* current = m_head;
	while (current != nullptr)
	{
		if (current->itemData.getName() == t_name)
		{
			return true;
		}
		current = current->next;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void InventorySystem::loadFromFile(const std::string& t_fileName)
{
	// Lambda function for converting a string to an EItemType
	auto stringToEnum = [](const std::string& t_typeStr) -> EItemType {
		if (t_typeStr == "Armor")
			return EItemType::ARMOR;
		if (t_typeStr == "Consumable")
			return EItemType::CONSUMABLE;
		if (t_typeStr == "Utility")
			return EItemType::UTILITY;
		if (t_typeStr == "Weapon")
			return EItemType::WEAPON;
		std::cerr << "Error: Invalid item type\n";
		return EItemType::ARMOR;
	};


	if (std::ifstream file(t_fileName); file.is_open())
	{
		std::string line;
		std::getline(file, line); // Skip the first line
		while (std::getline(file, line))
		{
			// Check if the line is just blank spaces
			bool isBlank = true;
			for (const char character : line)
			{
				if (character != ' ')
				{
					isBlank = false;
					break;
				}
			}

			if(!isBlank)
			{
				// Extract item properties from the line by finding the positions of the "//" characters
				const auto pos1 = line.find("//", 0);
				const auto pos2 = line.find("//", pos1 + 1);
				const auto pos3 = line.find("//", pos2 + 1);

				const std::string name = line.substr(0, pos1 - 1);
				const std::string type = line.substr(pos1 + 3, pos2 - pos1 - 4);
				const float price = std::stof(line.substr(pos2 + 3, pos3 - pos2 - 4));
				const int quantity = std::stoi(line.substr(pos3 + 3));

				// If the item does not exist in the inventory, add it
				if (!itemExists(name))
				{
					addItem(Item(name, stringToEnum(type), price, quantity));
					
				}
				else
				{
					std::cerr << "INFO: Item Import, Item duplicate found, Item already exists\n";
				}
			}

		}
		file.close();
	}
	else
	{
		std::cerr << "Error: Unable to open file." << std::endl;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Save the current inventory to the given file path
void InventorySystem::saveToFile(const std::string& t_fileName) const
{
	std::ofstream file(t_fileName);
	if (file.is_open())
	{
		file << "NAME // TYPE // PRICE // QUANTITY\n";
		const Node* current = m_head;
		while (current != nullptr)
		{
			const Item& item = current->itemData;
			// Lambda to convert the item type to a string - used because the EItemType enum is not printable and the function will only ever within this SaveToFile function 
			const std::string itemTypeStr = [&]() -> std::string {
				switch (item.getType())
				{
				case EItemType::ARMOR:      return "Armor";
				case EItemType::CONSUMABLE: return "Consumable";
				case EItemType::UTILITY:    return "Utility";
				case EItemType::WEAPON:     return "Weapon";
				}
				return {};
			}();

			// Write the item properties to the file
			file << item.getName() << " // " << itemTypeStr << " // "
				<< item.getPrice() << " // " << item.getQuantity() << "\n";

			current = current->next;
		}
		file.close();
	}
	else
	{
		std::cerr << "Error: Unable to open file path for writing." << std::endl;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Add a new item to the inventory
void InventorySystem::addItem(const Item& t_item)
{
	const auto newNode = new Node(t_item);

	// If the list is empty, set the new node as both the head and the tail
	if (m_head == nullptr)
	{
		m_head = newNode;
		m_tail = newNode;
	}
	else
	{
		// Otherwise, add the new node to the end of the list
		newNode->prev = m_tail;
		m_tail->next = newNode;
		m_tail = newNode;
	}

	m_size++;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Delete an item at the given index from the inventory
// Note that this function does not delete the item from memory, it simply
// removes the item from the linked list
void InventorySystem::deleteItemByIndex(const size_t t_index)
{
	if (t_index >= m_size || t_index < 0)
	{
		std::cerr << "Error: Invalid index\n";
		return;
	}

	const Node* current = m_head;
	size_t currentIndex = 0;
	while (current != nullptr)
	{
		if (currentIndex == t_index)
		{
			// Update the pointers of the adjacent nodes
			if (current == m_head && current == m_tail)
			{
				m_head = nullptr;
				m_tail = nullptr;
			}
			else if (current == m_head)
			{
				m_head = current->next;
				m_head->prev = nullptr;
			}
			else if (current == m_tail)
			{
				m_tail = current->prev;
				m_tail->next = nullptr;
			}
			else
			{
				current->prev->next = current->next;
				current->next->prev = current->prev;
			}

			delete current;
			m_size--;
			return;
		}

		current = current->next;
		currentIndex++;
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Edit an item at the given index in the inventory
// t_index: the index of the item to be edited
// t_newItem: the new item data to replace the old item data
void InventorySystem::editItem(const size_t t_index, const Item& t_newItem) const
{
	if (t_index >= m_size || t_index < 0)
	{
		std::cerr << "Error: Invalid index\n";
		return;
	}

	*itemAt(t_index) = t_newItem;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Sort the inventory by item name
// t_descending: true if the sort should be descending, false if the sort should be ascending
void InventorySystem::sortByName(const bool t_descending)
{
	quickSort(m_head, m_tail, t_descending, [](const Item& t_a, const Item& t_b)
		{ return t_a.getName() < t_b.getName(); });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Sort the inventory by item name
// t_descending: true if the sort should be descending, false if the sort should be ascending
void InventorySystem::sortByType(const bool t_descending)
{
	quickSort(m_head, m_tail, t_descending, [](const Item& t_a, const Item& t_b)
		{ return t_a.getType() < t_b.getType(); });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Sort the inventory by item name
// t_descending: true if the sort should be descending, false if the sort should be ascending
void InventorySystem::sortByPrice(const bool t_descending)
{
	quickSort(m_head, m_tail, t_descending, [](const Item& t_a, const Item& t_b)
		{ return t_a.getPrice() < t_b.getPrice(); });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Sort the inventory by item name
// t_descending: true if the sort should be descending, false if the sort should be ascending
void InventorySystem::sortByQuantity(const bool t_descending)
{
	quickSort(m_head, m_tail, t_descending, [](const Item& t_a, const Item& t_b)
		{ return t_a.getQuantity() < t_b.getQuantity(); });
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Get head node
// returns: a pointer to the head node of the linked list
InventorySystem::Node* InventorySystem::getHeadNode() const
{
	return m_head;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Get tail node
// returns: a pointer to the tail node of the linked list
InventorySystem::Node* InventorySystem::getTailNode() const
{
	return m_tail;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Get the size of the doubly linked list
//// returns: the number of elements in the the doubly linked list
size_t InventorySystem::getSize() const
{
	return m_size;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// function to set the on swap callback to update the GUI upon swapping an item
//
// t_callback: the callback function to set
void InventorySystem::setOnSwapCallback(const std::function<void()>& t_callback)
{
	m_onSwapCallback = t_callback;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Function to get the item at a given index
// 
// t_index: the index of the item to get
//
// returns: a pointer to the item at the given index
//
Item* InventorySystem::itemAt(const size_t t_index) const
{
	if (t_index >= m_size)
	{
		return nullptr;
	}

	if (m_head != nullptr)
	{
		Node* current = m_head;
		size_t currentIndex = 0;
		while (current != nullptr)
		{
			if (currentIndex == t_index)
			{
				return &current->itemData;
			}

			current = current->next;
			currentIndex++;
		}
	}
	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Function to swap two items in the inventory
// 
// t_node1: the first node to swap
// t_node2: the second node to swap
//
void InventorySystem::swapItems(Node* t_node1, Node* t_node2) const {
	const Item temp = t_node1->itemData;
	t_node1->itemData = t_node2->itemData;
	t_node2->itemData = temp;

	if (m_onSwapCallback) {
		m_onSwapCallback();
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// // Quick sort algorithm
// 
// t_low: the low end of the list to sort
// t_high: the high end of the list to sort
// t_sortDescending: if true, sort in descending order
// t_compareFunction: the function to use for comparing two items
//
void InventorySystem::quickSort(Node* t_low, Node* t_high, const bool t_sortDescending,
	const std::function<bool(const Item&, const Item&)>& t_compareFunction)
{
	if (t_high != nullptr && t_low != t_high && t_low != t_high->next)
	{
		const Node* p = partition(t_low, t_high, t_sortDescending, t_compareFunction);
		quickSort(t_low, p->prev, t_sortDescending, t_compareFunction);
		quickSort(p->next, t_high, t_sortDescending, t_compareFunction);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// Partition the list for the quick sort algorithm
// 
// t_low: the low end of the list to sort
// t_high: the high end of the list to sort
// t_sortDescending: if true, sort in descending order
// t_compareFunction: the function to use for comparing two items
InventorySystem::Node* InventorySystem::partition(Node* t_low, Node* t_high, const bool t_sortDescending,
	const std::function<bool(const Item&, const Item&)>& t_compareFunction) const {
	const Item pivot = t_high->itemData;
	Node* leftIndex = t_low->prev;

	for (Node* rightIndex = t_low; rightIndex != t_high; rightIndex = rightIndex->next)
	{
		if ((!t_sortDescending && t_compareFunction(rightIndex->itemData, pivot)) ||
			(t_sortDescending && t_compareFunction(pivot, rightIndex->itemData)))
		{
			leftIndex = (leftIndex == nullptr) ? t_low : leftIndex->next;
			swapItems(leftIndex, rightIndex);
		}
	}

	leftIndex = (leftIndex == nullptr) ? t_low : leftIndex->next;
	swapItems(leftIndex, t_high);
	return leftIndex;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
