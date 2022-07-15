#include "GameObject.h"
#include "CommonComponents.h"
#include <windows.h>

Scene::Scene(std::string tag)
{
	this->tag = tag;
}

GameObject Scene::CreateObject(std::string tag)
{
	for (auto [object, tagc] : registry.view<Tag>().each())
	{
		if (tagc.tag == tag)
		{
			MessageBox(0, L"GameObject with this name already exists", L"Error", MB_OK);
			return GameObject(entt::entity(0), this);
		}
	}
	GameObject object(registry.create(), this);
	object.AddComponent<Tag>(tag);
	object.AddComponent<Transformation>();
	return object;
}

void Scene::DestroyObject(std::string tag)
{
	for (auto [object, tagc] : registry.view<Tag>().each())
	{
		if (tagc.tag == tag)
			registry.destroy(object);
	}
}

void Scene::DestroyObject(GameObject object)
{
	registry.destroy(object.m_objHandle);
}

std::string Scene::GetTag() const
{
	return tag;
}