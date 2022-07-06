#include "ResourceManager.h"
#include "Primitives.h"
#include <d3dcompiler.h>

ResourceManager::ResourceManager()
{
	Primitives::InitializePrimitivesModels();
}

ResourceManager::~ResourceManager()
{
	Primitives::ReleasePrimitivesModels();
}

ID3DBlob* ResourceManager::GetShader(std::wstring szRcPath)
{
	ID3DBlob* pBlob = shaders[szRcPath];
	if (pBlob != NULL)
		return pBlob;
	if (D3DReadFileToBlob(szRcPath.c_str(), &pBlob) != S_OK)
		return NULL;
	shaders[szRcPath] = pBlob;
	return pBlob;
}

void ResourceManager::ReleaseShader(std::wstring szRcPath)
{
	auto pBlob = shaders[szRcPath];
	if (pBlob == NULL)
		return;
	pBlob->Release();
	shaders.erase(shaders.find(szRcPath));
}