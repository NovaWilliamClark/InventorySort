#pragma once
#include "Core/InventorySystem.hpp"
#include "Core/Gui.hpp"

class Application
{
public:
	Application();
	~Application();
	void run();

private:
	void update();
	void draw();

	const int m_screenWidth = 1172;
	const int m_screenHeight = 852;

	InventorySystem m_inventorySystem;
	Gui m_gui;

	raylib::Window m_window;
};

