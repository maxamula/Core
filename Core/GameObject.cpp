#include "GameObject.h"
#include "Mesh.h"

Scene::Scene(Graphics* gfx) : m_pGfx(gfx)
{}

GameObject Scene::CreateEntity(std::wstring tag)
{
	GameObject object(registry.create(), this);
	object.AddComponent<TagComponent>(tag);
	object.AddComponent<PositionComponent>(0.0f, 0.0f, 4.0f);
	return object;
}

void Scene::Update(float fDeltaTime)
{
	DrawObject();
}

GameObject::GameObject()
{}

GameObject::GameObject(entt::entity entity, Scene* pScene) : m_entHandle(entity), m_pScene(pScene)
{}

