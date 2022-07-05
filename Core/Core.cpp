#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#include "Core.h"

#define WIN32_LEAN_AND_MEAN

Core::Core()
{
	graphics = std::make_unique<Graphics>(this);
	m_scenes = std::make_unique<Scenes>(this);
}

void Core::LaunchAsync()
{
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Core::WindowThreadProxy, this, 0, 0));
}

void Core::Launch()
{
	WindowThread();
}

Graphics& Core::gfx()
{
	return *graphics.get();
}

Scenes& Core::scenes()
{
	return *m_scenes.get();
}

Window& Core::window()
{
	return *this->graphics->wnd.get();
}

Core::Input Core::input()
{
	return { keyboard, mouse };
}

ResourceManager& Core::resources()
{
	return m_resourceManager;
}

void Core::MainThreadProxy(Core* This)
{
	This->MainThread();
}

void Core::WindowThreadProxy(Core* This)
{
	This->WindowThread();
}

void Core::MainThread()
{
	
	while (graphics->wnd->GetWindowState())
	{
		graphics->BeginFrame();
		if (m_scenes->m_pCurrentScene != NULL)
		{
			m_scenes->m_pCurrentScene->Update(0.0f);
		}
		graphics->EndFrame();
	}
}

void Core::WindowThread()
{
	if (graphics->wnd->GetWindowState())
		ShowWindow(graphics->wnd->WinId(), SW_SHOWNORMAL);
	else
		ShowWindow(graphics->wnd->MakeWindow(), SW_SHOWNORMAL);
	// After creating window launch main thread
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThreadProxy, this, 0, 0));
	// Message loop
	MSG msg;
	while (GetMessage(&msg, graphics->wnd->m_hWnd, 0, 0) && graphics->wnd->GetWindowState())
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}