#include "GameObject.h"
#include "CommonComponents.h"

Scene::Scene(std::string tag)
{
	this->tag = tag;
}

GameObject Scene::CreateObject(std::string tag)
{
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