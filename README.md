# Inventory System

An inventory system for managing items in a store or game. The inventory system allows you to:

- Load items from a file
- Save items to a file
- Add and delete items
- Edit items
- Sort items by name, type, price, and quantity

## Features
### Inventory
The Inventory is a class that creates and stores a doubly linked list. This doubly linked list is made up of nodes, with each node storing items with their own properties.
We can traverse this list to find items, add, delete and modify the items.
### Loading and Saving Inventory
You can load and save the iventory to / from a txt file using the following format:
```
NAME // TYPE // PRICE // QUANTITY
```
### Adding, Deleting, and Editing Items
You can add, delete, and edit items in the inventory using the GUI.
Editing Item - If you want to edit an item use the spinner to change item index and fill in the form. When desired values are set hit "Modify Current Item"
Adding Item - If you want to add an item fill in the form. When desired values are set hit "Insert Item"
Delete Item - If you want to delete an item use the spinner to change item index and hit "Delete Current Item"

### Sorting Items
You can sort items in the inventory by Name, Type, Price, and Quantity in either Ascending or Descending order.
Select the respective dropboxes and hit "SORT". There is a delay on each swap made by a lambda callback function. This helps with viewing the sorting animation

### Item Types
There are four item types supported by the inventory system:

- Armor
- Consumable
- Utility
- Weapon