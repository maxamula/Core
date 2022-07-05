#pragma once
#include "Core.h"
#include <string>
#include "entt/entt.hpp"

class Core;
class GameObject;

struct TagComponent
{
	std::wstring tag;
};

struct PositionComponent
{
	float x, y, z;
};

class Scene
{
	friend class GameObject;
public:
	Scene(Core* core);
	~Scene() = default;

	GameObject CreateEntity(std::wstring tag);
	void Update(float fDeltaTime);
private:
	// Systems
	void DrawObject();

	entt::registry registry;
	Core* m_core;
};

class GameObject
{
public:
	GameObject();
	GameObject(entt::entity entity, Scene* pScene);

	template <typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return m_pScene->registry.emplace<T>(m_entHandle, args...);
	}

	template <typename T>
	T& GetComponent()
	{
		return m_pScene->registry.get<T>(m_entHandle);
	}

	template <typename T>
	void RemoveComponent()
	{
		return m_pScene->registry.remove<T>(m_entHandle);
	}

private:
	entt::entity m_entHandle = entt::entity(0);
	Scene* m_pScene = NULL;
};
