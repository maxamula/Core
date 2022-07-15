#pragma once
#include "entt/entt.hpp"
#include "CommonComponents.h"
#include <string>
#include <d3d11.h>

class Material	// entt wrapper
{
public:
	Material() = default;
	Material(entt::entity handle);

	static Material AddMaterial(std::string tag);
	static Material GetMaterial(std::string tag);
	static void DeleteMaterial(std::string tag);

	template <typename T, typename... Args>
	T& Add(Args&&... args)
	{
		return materials.emplace<T>(m_handle, args...);
	}

	template <typename T>
	T& Get()
	{
		return materials.get<T>(m_handle);
	}

	template <typename T>
	bool Has()
	{
		return materials.all_of<T>(m_handle);
	}

private:
	static entt::registry materials;	// holds material components
	entt::entity m_handle;
};

struct Color
{
	vec3 color = { 1.0f, 1.0f, 1.0f };	// solid color
};

struct ColorMap	// this will have higher priority over solid color if material contains both
{
	ID3D11ShaderResourceView* pTexture;	// pointer to a texture interface
};

