#include "Core.h"
#include "Window.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Debug.h"
#include "GameObject.h"

//imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// other
#include <list>

// Libraries
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Global declarations
Window window;
Graphics graphics;
Keyboard keyboard;
Mouse mouse;

// Collection of all game scenes
std::list<Scene*> scenes;
// Pointer to active scene
Scene* pActiveScene;

// Imgui stuff
ImGuiIO imGuiIO;

// Main game loop
void MainThread()
{
	while (window.GetWindowState())
	{
		graphics.BeginFrame();
		if (pActiveScene != NULL)
		{


			// draw imgui overlay
			DrawDebugWindow();
		}
		graphics.EndFrame();
	}
}

void Engine::InitializeEngine()
{
	// imgui initialization
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	imGuiIO = ImGui::GetIO();
	ImGui::StyleColorsDark();
	// Create window instance and show
	window.MakeWindow();
	ShowWindow(window.hWnd, SW_SHOWNORMAL);
	// Launch main game loop in separate thread
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0));
	// Message loop
	MSG msg;
	while (GetMessage(&msg, window.hWnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

// Window stuff

void Engine::SetWindowSize(unsigned short width, unsigned short height)
{
	// Set window size
	window.width = width;
	window.height = height;
	if (window.hWnd != NULL)
	{
		RECT oldPos;
		GetWindowRect(window.hWnd, &oldPos);
		oldPos.right = oldPos.left + width;
		oldPos.bottom = oldPos.top + height;
		AdjustWindowRectEx(&oldPos, WS_OVERLAPPEDWINDOW, 0, 0);
		if (SetWindowPos(window.hWnd, HWND_TOP, oldPos.left, oldPos.top, oldPos.right - oldPos.left, oldPos.bottom - oldPos.top, 0) == NULL)
			SetWindowPos(window.hWnd, HWND_TOP, oldPos.left, oldPos.top, oldPos.right - oldPos.left, oldPos.bottom - oldPos.top, SWP_ASYNCWINDOWPOS);
		// Resize resolution
		graphics.CreateInterfaces(window.hWnd);
	}
	// Set imgui coords
	imGuiIO.DisplaySize = {(float)width, (float)height};
}

unsigned short Engine::GetWindowWidth()
{
	return window.width;
}

unsigned short Engine::GetWindowHeight()
{
	return window.height;
}

void Engine::SetWindowCaption(std::wstring szCaption)
{
	SetWindowText(window.hWnd, szCaption.c_str());
	window.caption = szCaption;
}

std::wstring Engine::GetWindowCaption()
{
	return window.caption;
}

// Input
Keyboard& Engine::GetKeyboard()
{
	return keyboard;
}

Mouse& Engine::GetMouse()
{
	return mouse;
}

// Debug Windows

void Engine::ShowDebugWindow(DBG_WINDOW windowType)
{
	dbgWindow = windowType;
}

// Scenes control

Scene* Engine::CreateScene(std::string tag)
{
	for (auto scene : scenes)
	{
		if (scene->GetTag() == tag)
			return NULL;
	}
	Scene* newScene = new Scene(tag);
	scenes.push_back(newScene);
	return newScene;
}

Scene* Engine::GetScene(std::string tag)
{
	for (auto scene : scenes)
	{
		if (scene->GetTag() == tag)
			return scene;
	}
}

void Engine::DestroyScene(std::string tag)
{
	for (auto scene : scenes)
	{
		if (scene->GetTag() == tag)
		{
			scenes.remove(scene);
			delete scene;
			if (pActiveScene == scene)
				pActiveScene = NULL;
		}
	}
}

void Engine::SetActiveScene(std::string tag)
{
	for (auto scene : scenes)
	{
		if (scene->GetTag() == tag)
		{
			pActiveScene = scene;
		}
	}
}