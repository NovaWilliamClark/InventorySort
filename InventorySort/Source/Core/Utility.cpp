#include "Utility.hpp"

#include <Windows.h>

std::string Utility::getOpenFileName()
{
	// Initialize the OPENFILENAME struct and the buffer for the selected file path
	OPENFILENAME ofn = { 0 };
	wchar_t szFile[256] = { 0 };

	// Set the properties of the dialog box
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"text(.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Show the file explorer dialog box and get the selected file path
	if (GetOpenFileNameW(&ofn))
	{
		// Convert the selected file path from wide string to string and return it
		std::wstring fileName = ofn.lpstrFile;
		const auto rString = std::string(fileName.begin(), fileName.end());

		return rString;
	}
	return {};
}

std::string Utility::getSaveFileName()
{
	// Initialize the OPENFILENAME struct and the buffer for the selected file path
	OPENFILENAME ofn = { 0 };
	wchar_t szFile[260] = L"Untitled.txt";

	// Set the properties of the dialog box
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetConsoleWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"text(.txt)\0*.txt\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Show the file explorer dialog box and get the selected file path
	if (GetSaveFileNameW(&ofn))
	{
		// Convert the selected file path from wide string to string and return it
		std::wstring fileName = ofn.lpstrFile;
		const auto rString = std::string(fileName.begin(), fileName.end());

		return rString;
	}
	return {};
}
