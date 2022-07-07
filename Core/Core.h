#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "CommonComponents.h"
#include "GameObject.h"
#include <string>

// Enums
enum class DBG_WINDOW	// enum of debug windows
{
	DBG_WINDOW_NONE,
	DBG_WINDOW_TEST,
	DBG_WINDOW_SCENE
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
	//	Scenes&Levels
	Scene* CreateScene(std::string tag);
	Scene* GetScene(std::string tag);
	void DestroyScene(std::string tag);
	void SetActiveScene(std::string tag);

}



