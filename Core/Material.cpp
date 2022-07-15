#include "Material.h"
#include <Windows.h>

entt::registry Material::materials;

Material Material::AddMaterial(std::string tag)
{
	for (auto [object, tagc] : materials.view<Tag>().each())	// return empty material if material with same tag exists
	{
		if (tagc.tag == tag)
		{
			MessageBox(0, L"Material with this name already exists", L"Error", MB_OK);
			return Material();
		}
	}
	auto handle = materials.create();
	Material material(handle);
	materials.emplace<Tag>(handle, tag);
	return material;
}

Material Material::GetMaterial(std::string tag)
{
	for (auto [object, tagc] : materials.view<Tag>().each())
	{
		if (tagc.tag == tag)
		{
			return Material(object);
		}
	}
	return Material();
}

void Material::DeleteMaterial(std::string tag)
{
	// Cannot delete default material
	if (tag == "Default")
		return;
	for (auto [object, tagc] : materials.view<Tag>().each())
	{
		if (tagc.tag == tag)
		{
			materials.destroy(object);
		}
	}
}

Material::Material(entt::entity handle) : m_handle(handle)
{
}