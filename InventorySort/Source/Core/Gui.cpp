
// Include necessary header files
#include "Gui.hpp"
#include "Utility.hpp"

#define RAYGUI_IMPLEMENTATION
#include <raylib-cpp.hpp>
#include <raygui.h>
#include <iomanip>
#include <sstream>
#include <thread>

// Constructor for Gui class
Gui::Gui(InventorySystem& t_inventorySystem) : m_controlState(), m_inventorySystem(t_inventorySystem)
{}

// Destructor for Gui class
Gui::~Gui()
{
	// Unload every loaded Icon once program is closed and Gui class is destroyed off the stack.
	for (const auto& icon : m_itemIcons)
	{
		UnloadTexture(icon);
	}
}

void Gui::init()
{
	// Load item icons
	// NOTE: Textures MUST be loaded after Window initialization (OpenGL context is required)
	m_itemIcons[0] = LoadTexture("Content/Icons/Armor.png");
	m_itemIcons[1] = LoadTexture("Content/Icons/Consumable.png");
	m_itemIcons[2] = LoadTexture("Content/Icons/Utility.png");
	m_itemIcons[3] = LoadTexture("Content/Icons/Weapon.png");

	// Set swap callback for inventory system for updating the GUI when an item is swapped.
	m_inventorySystem.setOnSwapCallback([this]() {

		// Call the method to update the GUI and draw the inventory grid.
		m_windowRef.BeginDrawing();
		draw(m_windowRef);
		m_windowRef.EndDrawing();

		// Add a small delay to make the swap animation more visible.
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		});
}

// Update function for Gui class
void Gui::update()
{
	// Check for button presses and call corresponding functions if respective button is pressed.
	if (m_controlState.buttonModifyIsPressed)
	{
		buttonModifyItem();
	}

	if (m_controlState.buttonDeleteIsPressed)
	{
		buttonDeleteItem();
	}

	if (m_controlState.buttonInsertIsPressed)
	{
		buttonInsertItem();
	}

	if (m_controlState.buttonSortIsPressed)
	{
		buttonSortInventory();
	}

	if (m_controlState.buttonImportIsPressed)
	{
		buttonImportInventory();
	}
	if (m_controlState.buttonExportIsPressed)
	{
		buttonExportInventory();
	}
}

// Draw function for Gui class
void Gui::draw(const raylib::Window& t_window)
{
	// Draw
	//----------------------------------------------------------------------------------

	// Clear window background
	m_windowRef = t_window;
	m_windowRef.ClearBackground(raylib::Color::RayWhite());

	// raygui: controls drawing
	//----------------------------------------------------------------------------------
	// Draw controls
	if (m_controlState.dropboxEditTypeIsOpen || m_controlState.dropboxSortOrderIsOpen || m_controlState.dropboxSortValueIsOpen)
	{
		raygui::GuiLock();
	}

	// Draw Group Boxes
	raygui::GuiGroupBox(m_layoutRectangles[0], "Inventory");
	raygui::GuiGroupBox(m_layoutRectangles[9], "Options");

	// Draw Panels
	raygui::GuiPanel(m_layoutRectangles[13], nullptr);
	drawInventoryGrid();


	// Title Labels
	raygui::GuiLabel(m_layoutRectangles[14], "Current Item");
	raygui::GuiLabel(m_layoutRectangles[15], "New / Edit Item");

	// Draw Information Labels
	raygui::GuiLabel(m_layoutRectangles[16], "Name:");
	raygui::GuiLabel(m_layoutRectangles[17], "Type:");
	raygui::GuiLabel(m_layoutRectangles[18], "Price:");
	raygui::GuiLabel(m_layoutRectangles[19], "Quantity:");
	raygui::GuiLabel(m_layoutRectangles[20], "Mem Address:");

	// Draw Information Values
	drawItemInfo(m_controlState.spinnerSelectionIndexValue);

	// Draw
	raygui::GuiLabel(m_layoutRectangles[22], "Name:");
	raygui::GuiLabel(m_layoutRectangles[24], "Type:");
	raygui::GuiLabel(m_layoutRectangles[26], "Price:");
	raygui::GuiLabel(m_layoutRectangles[28], "Quantity:");
	raygui::GuiLabel(m_layoutRectangles[31], "Current Index");

	// Draw Sort Labels
	raygui::GuiLabel(m_layoutRectangles[37], "Sort Order:");
	raygui::GuiLabel(m_layoutRectangles[39], "Sort By Value:");

	// Draw spinner and value boxes
	if (raygui::GuiSpinner(m_layoutRectangles[32], nullptr, &m_controlState.spinnerSelectionIndexValue, 0, static_cast<int>(m_inventorySystem.getSize() - 1), m_controlState.spinnerSelectionIndexInEditMode))
	{
		m_controlState.spinnerSelectionIndexInEditMode = !m_controlState.spinnerSelectionIndexInEditMode;
	}
	if (raygui::GuiTextBox(m_layoutRectangles[23], m_controlState.textboxEditNameValue, 128, m_controlState.textboxEditNameIsInEditMode))
	{
		m_controlState.textboxEditNameIsInEditMode = !m_controlState.textboxEditNameIsInEditMode;
	}
	if (raygui::GuiValueBoxf(m_layoutRectangles[27], nullptr, &m_controlState.valueboxEditPriceValue, 0.f, 100000.f, m_controlState.valueboxEditPriceIsInEditMode))
	{
		m_controlState.valueboxEditPriceIsInEditMode = !m_controlState.valueboxEditPriceIsInEditMode;
	}
	if (raygui::GuiValueBox(m_layoutRectangles[29], nullptr, &m_controlState.valueboxEditQuantityValue, 1, 100000, m_controlState.valueboxEditQuantityIsInEditMode))
	{
		m_controlState.valueboxEditQuantityIsInEditMode = !m_controlState.valueboxEditQuantityIsInEditMode;
	}

	// Draw Lines
	raygui::GuiLine(m_layoutRectangles[7], nullptr);
	raygui::GuiLine(m_layoutRectangles[21], nullptr);
	raygui::GuiLine(m_layoutRectangles[33], nullptr);

	// Draw Buttons
	m_controlState.buttonModifyIsPressed = raygui::GuiButton(m_layoutRectangles[10], "Modify Current Item");
	m_controlState.buttonInsertIsPressed = raygui::GuiButton(m_layoutRectangles[11], "Insert  Item");
	m_controlState.buttonDeleteIsPressed = raygui::GuiButton(m_layoutRectangles[12], "Delete current Item");
	m_controlState.buttonSortIsPressed = raygui::GuiButton(m_layoutRectangles[34], "Sort Inventory");
	m_controlState.buttonImportIsPressed = raygui::GuiButton(m_layoutRectangles[35], "Import List");
	m_controlState.buttonExportIsPressed = raygui::GuiButton(m_layoutRectangles[36], "Export List");

	// Draw Dropboxes
	if (raygui::GuiDropdownBox(m_layoutRectangles[25], "Armor;Consumable;Utility;Weapon", &m_controlState.dropboxEditTypeValue, m_controlState.dropboxEditTypeIsOpen)) m_controlState.dropboxEditTypeIsOpen = !m_controlState.dropboxEditTypeIsOpen;
	if (raygui::GuiDropdownBox(m_layoutRectangles[40], "Name;Type;Price;Quantity", &m_controlState.dropboxSortValueValue, m_controlState.dropboxSortValueIsOpen)) m_controlState.dropboxSortValueIsOpen = !m_controlState.dropboxSortValueIsOpen;
	if (raygui::GuiDropdownBox(m_layoutRectangles[38], "Ascending;Descending", &m_controlState.dropboxSortOrderValue, m_controlState.dropboxSortOrderIsOpen)) m_controlState.dropboxSortOrderIsOpen = !m_controlState.dropboxSortOrderIsOpen;

	// Check for errors and display error message if necessary
	if (m_controlState.errorMessageIsOpen)
	{
		if (const auto result = raygui::GuiMessageBox(raylib::Rectangle{ m_windowRef.GetWidth() / 2.f - 200, m_windowRef.GetHeight() / 2.f - 100, 400, 200 }, GuiIconText(raygui::ICON_DEMON, "Error"), "Item Already Exists Within List", "OK"); result == 1)
		{
			m_controlState.errorMessageIsOpen = false;
		}
	}

	raygui::GuiUnlock();
	//----------------------------------------------------------------------------------
}

// Function to modify an item in the inventory
void Gui::buttonModifyItem()
{
	// Check if the new name already exists in the inventory, if it does, check we are trying to modify the same item, if not error
	if (m_inventorySystem.itemExists(m_controlState.textboxEditNameValue) && m_inventorySystem.itemAt(m_controlState.spinnerSelectionIndexValue)->getName() != m_controlState.textboxEditNameValue)
	{
		// Open Error Message
		m_controlState.errorMessageIsOpen = true;
		return;
	}
	// Create a temporary item that will hold the modified items data (basically new items data)

	const Item tempItem(m_controlState.textboxEditNameValue, static_cast<EItemType>(m_controlState.dropboxEditTypeValue), m_controlState.valueboxEditPriceValue, m_controlState.valueboxEditQuantityValue);
	// Update the item in the inventory system
	m_inventorySystem.editItem(m_controlState.spinnerSelectionIndexValue, tempItem);
}

void Gui::buttonInsertItem()
{
	// Check if the new name already exists in the inventory
	if (m_inventorySystem.itemExists(m_controlState.textboxEditNameValue))
	{
		// Open Error Message
		m_controlState.errorMessageIsOpen = true;
		return;
	}

	// Create a temporary item that will hold the new items data
	const Item tempItem(m_controlState.textboxEditNameValue, static_cast<EItemType>(m_controlState.dropboxEditTypeValue), m_controlState.valueboxEditPriceValue, m_controlState.valueboxEditQuantityValue);

	// Update the item in the inventory system
	m_inventorySystem.addItem(tempItem);
}

// Function to delete an item from the inventory
void Gui::buttonDeleteItem() const
{
	m_inventorySystem.deleteItemByIndex(m_controlState.spinnerSelectionIndexValue);
}


// Function to sort the inventory based on user-selected criteria
void Gui::buttonSortInventory() const
{
	switch (m_controlState.dropboxSortValueValue)
	{
	case 0:	m_inventorySystem.sortByName(m_controlState.dropboxSortOrderValue);		break;
	case 1:	m_inventorySystem.sortByType(m_controlState.dropboxSortOrderValue);		break;
	case 2:	m_inventorySystem.sortByPrice(m_controlState.dropboxSortOrderValue);	break;
	case 3:	m_inventorySystem.sortByQuantity(m_controlState.dropboxSortOrderValue);	break;
	default:;
	}
}


// Function to import an inventory list from a file
void Gui::buttonImportInventory() const
{
	if (const auto path = Utility::getOpenFileName(); !path.empty())
	{
		m_inventorySystem.loadFromFile(path);
	}
	else
	{
		std::cerr << "Could Not Open File Path" << std::endl;
	}
}

// Function to export the inventory list to a file
void Gui::buttonExportInventory()
{
	if (const auto path = Utility::getSaveFileName(); !path.empty())
	{
		m_inventorySystem.saveToFile(path);
	}
	else
	{
		std::cerr << "Could Not Open File Path" << std::endl;
	}
}

// Function to draw item information on the right hand information panel GUI
void Gui::drawItemInfo(const int t_index) const
{

	auto enumToStr = [](const EItemType t_type) -> std::string {
		if (t_type == EItemType::ARMOR)
		{
			return "Armor";
		}
		else if (t_type == EItemType::CONSUMABLE)
		{
			return "Consumable";
		}
		else if (t_type == EItemType::UTILITY)
		{
			return "Utility";
		}
		else if (t_type == EItemType::WEAPON)
		{
			return "Weapon";
		}
		else
		{
			return "Unknown";
		}
	};

	if (const Item* currentItem = m_inventorySystem.itemAt(t_index); currentItem != nullptr)
	{
		const std::string itemName = currentItem->getName();
		const std::string itemType = enumToStr(currentItem->getType());
		const std::string itemPrice = "$" + floatToStringWithPrecision(currentItem->getPrice(), 2);
		const std::string itemQuantity = std::to_string(currentItem->getQuantity());
		const std::string itemMemoryAddress = formatMemoryAddress(currentItem->getMemoryAddress());

		// Draw the labels
		raygui::GuiLabel(raylib::Rectangle{ m_anchor.controlsAnchor.x + 48, m_anchor.controlsAnchor.y + 64, 232, 24 }, itemName.c_str());
		raygui::GuiLabel(raylib::Rectangle{ m_anchor.controlsAnchor.x + 48, m_anchor.controlsAnchor.y + 104, 232, 24 }, itemType.c_str());
		raygui::GuiLabel(raylib::Rectangle{ m_anchor.controlsAnchor.x + 48, m_anchor.controlsAnchor.y + 144, 120, 24 }, itemPrice.c_str());
		raygui::GuiLabel(raylib::Rectangle{ m_anchor.controlsAnchor.x + 64, m_anchor.controlsAnchor.y + 184, 120, 24 }, itemQuantity.c_str());
		raygui::GuiLabel(raylib::Rectangle{ m_anchor.controlsAnchor.x + 88, m_anchor.controlsAnchor.y + 224, 120, 24 }, itemMemoryAddress.c_str());
	}
}

// Function to draw the inventory grid
void Gui::drawInventoryGrid()
{
	// Draw inventory labels and row backgrounds
	int rowIndex = 0;
	m_controlState.contentRect = raylib::Rectangle{ 0, 0, m_layoutRectangles[8].width, 30 * static_cast<float>(m_inventorySystem.getSize()) };
	m_controlState.scrollPanelView = raygui::GuiScrollPanel({ m_layoutRectangles[8].x, m_layoutRectangles[8].y, m_layoutRectangles[8].width - m_controlState.scrollPanelBoundsOffset.x, m_layoutRectangles[8].height - m_layoutRectangles[8].y }, nullptr, m_controlState.contentRect, &m_controlState.scrollPanelScrollOffset);

	// Draw Inventory Labels
	raygui::GuiLabel(m_layoutRectangles[1], "Index:");
	raygui::GuiLabel(m_layoutRectangles[2], "Name:");
	raygui::GuiLabel(m_layoutRectangles[3], "Type:");
	raygui::GuiLabel(m_layoutRectangles[4], "Mem Address:");
	raygui::GuiLabel(m_layoutRectangles[5], "Price:");
	raygui::GuiLabel(m_layoutRectangles[6], "Quantity:");

	const InventorySystem::Node* currentNode = m_inventorySystem.getHeadNode();
	while (currentNode != nullptr) {
		constexpr int PADDING = 5;
		constexpr int ROW_HEIGHT = 25;
		constexpr int HEADER_HEIGHT = 30;
		Item item = currentNode->itemData;

		// Setup the content rectangle bound. Content Rectangle will show the displayed items within the scroll view. Check if an item has been scrolled in/out of the bounds of the scroll panel and display / hide it accordingly
		if (const float posY = m_controlState.contentRect.y + HEADER_HEIGHT + static_cast<float>(ROW_HEIGHT * rowIndex) + (m_controlState.scrollPanelScrollOffset.y + 50);
			static_cast<float>(posY) + ROW_HEIGHT > (m_controlState.scrollPanelView.y + 25) && posY < m_controlState.scrollPanelView.y + (m_controlState.scrollPanelView.height - 15)) {
			if (rowIndex % 2 == 0) {
				DrawRectangle(static_cast<int>(m_controlState.scrollPanelView.x), static_cast<int>(posY), static_cast<int>(m_controlState.scrollPanelView.width), ROW_HEIGHT, LIGHTGRAY);
			}

			// Draw the index, name, price, quantity, and memory address for each item
			char indexStr[16];
			snprintf(indexStr, sizeof(indexStr), "%d", rowIndex);
			DrawText(indexStr, static_cast<int>(m_controlState.scrollPanelView.x) + PADDING * 2, static_cast<int>(posY + PADDING), 18, BLACK);
			DrawText(item.getName().c_str(), static_cast<int>(m_controlState.scrollPanelView.x) + 120, static_cast<int>(posY + PADDING), 18, BLACK);
			DrawText(formatMemoryAddress(currentNode->itemData.getMemoryAddress()).c_str(), static_cast<int>(m_controlState.scrollPanelView.x) + 372, static_cast<int>(posY + PADDING), 18, BLACK);
			char priceStr[32];
			snprintf(priceStr, sizeof(priceStr), "$%.2f", static_cast<float>(item.getPrice()));
			DrawText(priceStr, static_cast<int>(m_controlState.scrollPanelView.x) + 555, static_cast<int>(posY + PADDING), 18, BLACK);
			char quantityStr[32];
			snprintf(quantityStr, sizeof(quantityStr), "%d", item.getQuantity());
			DrawText(quantityStr, static_cast<int>(m_controlState.scrollPanelView.x) + 686, static_cast<int>(posY + PADDING), 18, BLACK);

			// Draw the item type as a colored icon
			const EItemType itemType = item.getType();
			const Color iconColor = getIconColor(itemType);
			const int posX = static_cast<int>(m_controlState.scrollPanelView.x) + 72;
			DrawTexture(getIconTexture(itemType), posX, static_cast<int>(posY + PADDING), iconColor);
		}
		currentNode = currentNode->next;
		rowIndex++;
	}
}


// Get the texture for the item type icon
raylib::Texture2D& Gui::getIconTexture(const EItemType t_item)
{
	switch (t_item) {
	case EItemType::ARMOR:		return m_itemIcons[0];
	case EItemType::CONSUMABLE:	return m_itemIcons[1];
	case EItemType::UTILITY:	return m_itemIcons[2];
	case EItemType::WEAPON:		return m_itemIcons[3];
	}
	return m_itemIcons[0];
}

// Get the color for the item type icon
raylib::Color Gui::getIconColor(const EItemType t_type) {
	switch (t_type) {
	case EItemType::ARMOR:		return raylib::Color::Yellow();
	case EItemType::CONSUMABLE:	return raylib::Color::Blue();
	case EItemType::UTILITY:	return raylib::Color::Purple();
	case EItemType::WEAPON:	return raylib::Color::Red();
	}
	return raylib::Color::RayWhite();
}

// Utility function to format the memory address of an item
std::string Gui::formatMemoryAddress(const std::string& t_address) const
{
	std::stringstream ss;
	std::string str = t_address;
	str = str.substr(5, std::string::npos); // Remove the "00000" part of the address
	ss << "0x" << str;
	return ss.str();
}

// Format a float to a string with a specified precision
std::string Gui::floatToStringWithPrecision(const float t_value, const int t_precision) const
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(t_precision) << t_value;
	return ss.str();
}

