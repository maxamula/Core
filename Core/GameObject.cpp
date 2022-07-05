#include "GameObject.h"
#include "Mesh.h"

Scene::Scene(Core* core) : m_core(core)
{}

GameObject Scene::CreateEntity(std::wstring tag)
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

