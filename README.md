## Getting Started
Minimal code example:
```
 #pragma comment(lib, "Core.lib")
#include <Core.h>

Core core;  //Game core

void InitializeGame()
{
    // Window setup
    core.window().SetCaption((WCHAR*)L"Example Game");
    core.window().SetWindowSize(1600, 900);
    core.gfx().SetResolution({1600, 900});
    
    // Set up scene
    auto pMainLevel = core.scenes().CreateScene(L"Main Level");
    auto cube = Cube(pMainLevel, L"Cube1");
    cube.GetComponent<PositionComponent>() = { 0.0f, 0.0f, 6.0f };
    cube.GetComponent<MeshRenderer>().rotation = { 0.7f, 0.3f, 0.0f };

    // Set main level as current level
    core.scenes().SetCurrentLevel(L"Main Level");
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
    InitializeGame();
    core.Launch();
}
```
Code execution result:
![image](https://user-images.githubusercontent.com/78308778/176960173-fd0388f6-bd67-4865-8ee1-70c48b862da5.png)

