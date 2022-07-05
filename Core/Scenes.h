#pragma once
#include "Core.h"
#include "GameObject.h"
#include <map>
#include <string>

class Core;
class Scene;

class Scenes
{
	friend class Core;
public:
	Scenes(Core* core);
	Scene* CreateScene(std::wstring szLevelName);
	Scene* GetScene(std::wstring szLevelName);
	void DeleteScene(std::wstring szLevelName);
	void SetCurrentLevel(std::wstring szLevelName);
private:
	Core* m_core;
	std::map<std::wstring, Scene*> m_sceneList;
	Scene* m_pCurrentScene = NULL;
};

