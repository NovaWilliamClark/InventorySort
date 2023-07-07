#include "Application.hpp"

Application::Application() : m_gui(m_inventorySystem)
{
	m_window.Init(m_screenWidth, m_screenHeight, "Inventory System - William Clark");
	m_window.SetTargetFPS(60);
}

Application::~Application()
{
	m_window.Close();
}

void Application::run()
{
	m_gui.init();
	while (!m_window.ShouldClose())
	{
		update();
		draw();
	}
}

void Application::update()
{
	m_gui.update();
}

void Application::draw()
{
	m_window.BeginDrawing();

	m_gui.draw(m_window);

	m_window.EndDrawing();
}
