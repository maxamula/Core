#include "GameObject.h"
#include "Mesh.h"

Scene::Scene(Core* core) : m_core(core)
{}

GameObject Scene::CreateObject(std::wstring tag)
{
	GameObject object(registry.create(), this);
	object.AddComponent<TagComponent>(tag);
	object.AddComponent<PositionComponent>(0.0f, 0.0f, 4.0f);
	return object;
}

Camera& Scene::GetCamera()
{
	return m_activeCamera;
}

GameObject Scene::Object(std::wstring tag)
{
	for (auto [object, tagc] : registry.view<TagComponent>().each())
	{
		if (tagc.tag == tag)
			return GameObject(object, this);
	}
	return GameObject();
}

void Scene::DestroyObject(std::wstring tag)
{
	for (auto [object, tagc] : registry.view<TagComponent>().each())
	{
		if (tagc.tag == tag)
			registry.destroy(object);
	}
}

void Scene::DestroyObject(GameObject object)
{
	registry.destroy(object.m_entHandle);
}

void Scene::Start()
{
	ScriptStart();
}

void Scene::Update(float fDeltaTime)
{
	DrawScene();
	ScriptUpdate(fDeltaTime);
}

GameObject::GameObject()
{}

GameObject::GameObject(entt::entity entity, Scene* pScene) : m_entHandle(entity), m_pScene(pScene)
{}

