#pragma once
#include "GameObject.h"
#include <map>
#include <string>

class Scene;
class Graphics;

class Scenes
{
	friend class Core;
public:
	Scenes(Graphics* pGfx);
	Scene* CreateScene(std::wstring szLevelName);
	Scene* GetScene(std::wstring szLevelName);
	void DeleteScene(std::wstring szLevelName);
	void SetCurrentLevel(std::wstring szLevelName);
private:
	Graphics* m_pGfx;
	std::map<std::wstring, Scene*> m_sceneList;
	Scene* m_pCurrentScene = NULL;
};

