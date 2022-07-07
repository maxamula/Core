#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include <string>

// Enums
enum class DBG_WINDOW	// enum of debug windows
{
	DBG_WINDOW_NONE,
	DBG_WINDOW_TEST
};

namespace Engine
{
	void InitializeEngine();
	// Window stuff
	void SetWindowSize(unsigned short width, unsigned short height);
	unsigned short GetWindowWidth();
	unsigned short GetWindowHeight();
	void SetWindowCaption(std::wstring szCaption);
	std::wstring GetWindowCaption();
	// Input
	Keyboard& GetKeyboard();
	Mouse& GetMouse();
	// Debug windows
	void ShowDebugWindow(DBG_WINDOW windowType);

}



