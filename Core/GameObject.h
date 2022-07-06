#pragma once
#include "Core.h"
#include "Camera.h"
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

	GameObject CreateObject(std::wstring tag);
	Camera& GetCamera();
	GameObject Object(std::wstring tag);
	void DestroyObject(std::wstring tag);
	void DestroyObject(GameObject object);
	void Start();
	void Update(float fDeltaTime);
private:
	// Systems
	void DrawScene();
	void ScriptStart();
	void ScriptUpdate(float fElapsedTime);

	entt::registry registry;
	Core* m_core;
	Camera m_activeCamera;
};

class GameObject
{
	friend class Scene;
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
