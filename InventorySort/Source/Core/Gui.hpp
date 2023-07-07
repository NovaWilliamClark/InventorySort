#pragma once

#include <raylib-cpp.hpp>
#include "InventorySystem.hpp"
#include "Item.hpp"

class Gui
{
public:
	explicit Gui(InventorySystem& t_inventorySystem);
	Gui() = delete;
	~Gui();

	void init();
	void update();
	void draw(const raylib::Window& t_window);

private:

	// BUTTONS
	void buttonModifyItem();
	void buttonInsertItem();
	void buttonDeleteItem() const;
	void buttonSortInventory() const;
	void buttonImportInventory() const;
	void buttonExportInventory();

	void drawItemInfo(int t_index) const;
	void drawInventoryGrid();

	raylib::Texture2D& getIconTexture(EItemType t_item);
	raylib::Color getIconColor(EItemType t_type);

	[[nodiscard]] std::string formatMemoryAddress(const std::string& t_address) const;
	[[nodiscard]] std::string floatToStringWithPrecision(float t_value, int t_precision) const;

	//----------------------------------------------------------------------------------
	// Define anchors
	//----------------------------------------------------------------------------------
	struct Anchor
	{
		Vector2 inventoryAnchor{ 40, 24 }; // Anchor ID: 1
		Vector2 controlsAnchor{ 832, 16 }; // Anchor ID: 2
	};
	Anchor m_anchor;

	//----------------------------------------------------------------------------------
	// Define Gui Control Variables
	//----------------------------------------------------------------------------------
	struct ControlState
	{
		// Scroll Panel //
		raylib::Rectangle scrollPanelView {0 ,0 ,0,0};
		raylib::Vector2 scrollPanelScrollOffset {0, 0};
		raylib::Vector2 scrollPanelBoundsOffset {0, 0};

		raylib::Rectangle contentRect = { 0, 0, 0, 0 };

		// Dropboxes //
		int dropboxEditTypeValue;		// New / Edit item type dropbox
		bool dropboxEditTypeIsOpen;

		int dropboxSortOrderValue;		// Sort Order dropbox (Ascending, Descending)
		bool dropboxSortOrderIsOpen;

		int dropboxSortValueValue;		// Sort Value dropbox (Name, Type, Price, Quantity)
		bool dropboxSortValueIsOpen;

		// Spinners //
		int spinnerSelectionIndexValue;	// Item selection Index spinner
		bool spinnerSelectionIndexInEditMode;

		// Text Boxes //
		char textboxEditNameValue[128] = "Enter Name";	// Edit Item Name textbox value
		bool textboxEditNameIsInEditMode;

		// Value Boxes //
		float valueboxEditPriceValue;		// Edit Item Price valuebox value
		bool valueboxEditPriceIsInEditMode;

		int valueboxEditQuantityValue;		// Edit Item Quantity valuebox value
		bool valueboxEditQuantityIsInEditMode;

		// Buttons
		bool buttonModifyIsPressed;
		bool buttonInsertIsPressed;
		bool buttonDeleteIsPressed;
		bool buttonSortIsPressed;
		bool buttonImportIsPressed;
		bool buttonExportIsPressed;

		// Error Message
		bool errorMessageIsOpen;
	};
	ControlState m_controlState;

	//----------------------------------------------------------------------------------
	// Define Gui Rectangle sizes and positions
	//----------------------------------------------------------------------------------
	const raylib::Rectangle m_layoutRectangles[41] =
	{
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 0, m_anchor.inventoryAnchor.y + 0, 776, 784 },		// INVENTORY GROUP BOX				//0
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 8, m_anchor.inventoryAnchor.y + 24, 32, 24 },		// INDEX LABEL						//1
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 128, m_anchor.inventoryAnchor.y + 24, 120, 24 },	// NAME LABEL						//2
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 72, m_anchor.inventoryAnchor.y + 24, 32, 24 },		// TYPE LABEL						//3
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 400, m_anchor.inventoryAnchor.y + 24, 120, 24 },	// MEM ADDRESS LABEL				//4
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 568, m_anchor.inventoryAnchor.y + 24, 48, 24 },		// PRICE LABEL						//5
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 688, m_anchor.inventoryAnchor.y + 24, 56, 24 },		// QUANTITY LABEL					//6
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 0, m_anchor.inventoryAnchor.y + 32, 776, 24 },		// INVENTORY LINE					//7
		raylib::Rectangle{ m_anchor.inventoryAnchor.x + 0, m_anchor.inventoryAnchor.y + 48, 776, 810 },		// INVENTORY SCROLL PANEL			//8
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 0, m_anchor.controlsAnchor.y + 8, 296, 784 },		// OPTION GROUP BOX					//9
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 168, m_anchor.controlsAnchor.y + 480, 120, 24 },		// MODIFY ITEM BUTTON				//10
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 168, m_anchor.controlsAnchor.y + 512, 120, 24 },		// INSERT ITEM BUTTON				//11
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 168, m_anchor.controlsAnchor.y + 544, 120, 24 },		// DELETE ITEM BUTTON				//12
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 0, m_anchor.controlsAnchor.y + 40, 296, 216 },		// INFORMATION PANEL				//13
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 104, m_anchor.controlsAnchor.y + 16, 120, 24 },		// CURRENT ITEM INFO LABEL			//14
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 96, m_anchor.controlsAnchor.y + 256, 120, 24 },		// NEW / EDIT ITEM LABEL			//15
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 64, 120, 24 },		// INFO NAME LABEL					//16
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 104, 120, 24 },		// INFO TYPE LABEL					//17
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 144, 120, 24 },		// INFO PRICE LABEL					//18
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 184, 120, 24 },		// INFO QUANTITY LABEL				//19
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 224, 120, 24 },		// INFO MEMORY ADDRESS LABEL		//20
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 0, m_anchor.controlsAnchor.y + 272, 296, 16 },		// NEW / EDIT TOP LINE				//21
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 304, 40, 24 },		// EDIT NAME LABEL					//22
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 64, m_anchor.controlsAnchor.y + 304, 224, 24 },		// EDIT NAME TEXTBOX				//23
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 336, 40, 24 },		// EDIT TYPE LABEL					//24
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 64, m_anchor.controlsAnchor.y + 336, 224, 24 },		// EDIT TYPE DROPBOX				//25
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 368, 40, 24 },		// EDIT PRICE LABEL					//26
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 64, m_anchor.controlsAnchor.y + 368, 224, 24 },		// EDIT PRICE VALUE BOX				//27
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 400, 64, 24 },		// EDIT QUANTITY LABEL				//28
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 64, m_anchor.controlsAnchor.y + 400, 224, 24 },		// EDIT QUANTITY VALUEBOX			//29
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 0, m_anchor.controlsAnchor.y + 432, 296, 16 },		// NEW / EDIT BOTTOM LINE			//30
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 46, m_anchor.controlsAnchor.y + 462, 82, 24 },		// CURRENT INDEX LABEL				//31
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 16, m_anchor.controlsAnchor.y + 480, 136, 88 },		// CURRENT INDEX SPINNER			//32
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 0, m_anchor.controlsAnchor.y + 576, 296, 32 },		// SORT TOP LINE					//33
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 104, m_anchor.controlsAnchor.y + 696, 184, 88 },		// SORT BUTTON						//34
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 696, 88, 40 },		// IMPORT BUTTON					//35
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 744, 88, 40 },		// EXPORT BUTTON					//36
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 616, 96, 32 },		// SORT ORDER LABEL					//37
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 104, m_anchor.controlsAnchor.y + 616, 184, 32 },		// SORT ORDER DROPDOWN				//38
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 8, m_anchor.controlsAnchor.y + 656, 96, 32 },		// SORT BY LABEL					//39
		raylib::Rectangle{ m_anchor.controlsAnchor.x + 104, m_anchor.controlsAnchor.y + 656, 184, 32 }		// SORT By DROPDOWN					//40
	};

	InventorySystem& m_inventorySystem;

	raylib::Texture2D m_itemIcons[4]{};
	raylib::Window m_windowRef;
};

