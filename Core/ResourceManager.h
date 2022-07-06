#pragma once
#include "Mesh.h"
#include <d3d11.h>
#include <string>
#include <unordered_map>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	ID3DBlob* GetShader(std::wstring szRcPath);
	void ReleaseShader(std::wstring szRcPath);
private:
	std::unordered_map<std::wstring, ID3DBlob*> shaders;
	std::unordered_map<std::wstring, Mesh*> meshes;
};

