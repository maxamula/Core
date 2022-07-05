#pragma once
#include "Graphics.h"
#include "Scenes.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ResourceManager.h"
#include <memory>

class Graphics;
class Window;
class Scenes;

class Core
{
public:
	struct Input
	{
		Keyboard& keyboard;
		Mouse& mouse;
	};
public:
	Core();
	void LaunchAsync();
	void Launch();
	Graphics& gfx();
	Scenes& scenes();
	Window& window();
	Input input();
	ResourceManager& resources();
	
	
protected:
	// Graphics
	std::unique_ptr<Graphics> graphics;
	std::unique_ptr<Scenes> m_scenes;
	// Input
	Keyboard keyboard;
	Mouse mouse;
	// Resource manager
	ResourceManager m_resourceManager;
private:
	static void MainThreadProxy(Core* This);
	static void WindowThreadProxy(Core* This);
	void MainThread();
	void WindowThread();
};
 
