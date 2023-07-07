#pragma once
#include "Item.hpp"

#include <functional>
#include <iostream>

class InventorySystem
{
public:
	// Node class for the doubly-linked list
	class Node {
	public:
		explicit Node(const Item& t_newItem, Node* t_next = nullptr, Node* t_prev = nullptr);
		Item itemData;
		Node* next;
		Node* prev;
	};

	// Constructor and Destructor
	InventorySystem();
	~InventorySystem();

	[[nodiscard]] bool itemExists(const std::string& t_name) const;
	void loadFromFile(const std::string& t_fileName);
	void saveToFile(const std::string& t_fileName) const;

	// Methods for adding, deleting, and editing items
	void addItem(const Item& t_item);
	void deleteItemByIndex(const size_t t_index);
	void editItem(const size_t t_index, const Item& t_newItem) const;

	// Sorting functions
	void sortByName(bool t_descending);
	void sortByType(bool t_descending);
	void sortByPrice(bool t_descending);
	void sortByQuantity(bool t_descending);

	[[nodiscard]] Node* getHeadNode() const;
	[[nodiscard]] Node* getTailNode() const;
	[[nodiscard]] size_t getSize() const;

	void setOnSwapCallback(const std::function<void()>& t_callback);

	[[nodiscard]] Item* itemAt(const size_t t_index) const;

private:
	Node* m_head;
	Node* m_tail;
	size_t m_size;

	// Callback function for when the inventory swap occurs
	std::function<void()> m_onSwapCallback;

	// Helper method for sorting the inventory using QuickSort algorithm
	void swapItems(Node* t_node1, Node* t_node2) const;
	void quickSort(Node* t_low, Node* t_high, bool t_sortDescending, const std::function<bool(const Item&, const Item&)>&
	               t_compareFunction);
	Node* partition(Node* t_low, Node* t_high, bool t_sortDescending, const std::function<bool(const Item&, const Item&)>
	                & t_compareFunction) const;
};

