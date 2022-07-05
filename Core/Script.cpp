#include "Script.h"

void Scene::ScriptStart()
{
	for (auto [object, script] : registry.view<ScriptComponent>().each())
	{
		script.pScript->Start(GameObject(object, this));
	}
}

void Scene::ScriptUpdate(float fElapsedTime)
{
	for (auto [object, script] : registry.view<ScriptComponent>().each())
	{
		script.pScript->Update(fElapsedTime, GameObject(object, this));
	}
}