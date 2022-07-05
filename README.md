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

## Creating a first script
Creating a script that will rotate cube while keybind is pressed
Script class
```
class CubeScript : public Script
{
public:
	void Start(GameObject object) override;
	void Update(float fElapsedTime, GameObject object) override;
};
```
Script body:
```
void CubeScript::Start(GameObject object)
{

}

void CubeScript::Update(float fElapsedTime, GameObject object)
{
	if (core.input().keyboard.KeyPressed(VK_F1))
		object.GetComponent<MeshRenderer>().rotation.x += 0.1;
}
```
Result:
![2022-07-06 02-16-31](https://user-images.githubusercontent.com/78308778/177432805-6811b5dc-b9fa-4020-9877-ed46a1aa2c73.gif)

