#pragma once
#include "entt/entt.hpp"
#include <string>

class GameObject;

class Scene
{
	friend class GameObject;
	friend void SceneDbgWindow(void);
	friend class Graphics;
public:
	Scene(std::string tag);

	GameObject CreateObject(std::string tag);
	void DestroyObject(std::string tag);
	void DestroyObject(GameObject object);

	std::string GetTag() const;
private:
	entt::registry registry;
	std::string tag;
};

class GameObject
{
	friend class Scene;
public:
	GameObject(entt::entity objectHandle, Scene* pScene);

	template <typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return m_pScene->registry.emplace<T>(m_objHandle, args...);
	}

	template <typename T>
	T& GetComponent()
	{
		return m_pScene->registry.get<T>(m_objHandle);
	}

	template <typename T>
	void RemoveComponent()
	{
		return m_pScene->registry.remove<T>(m_objHandle);
	}

	template <typename T>
	bool HasComponent()
	{
		return m_pScene->registry.all_of<T>(m_objHandle);
	}

private:
	entt::entity m_objHandle = entt::entity(0);
	Scene* m_pScene = NULL;
};

