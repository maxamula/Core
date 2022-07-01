#pragma once
#include "Vector.h"
#include <string>
#include <map>
#include <wrl/client.h>

struct Vertex
{
	float x, y, z;
};

struct Mesh
{
	Vertex* pVertexData;
	size_t vertices;
	unsigned int* pIndexData;
	size_t indices;
};

// Mesh loader
Mesh GetMesh(std::wstring szPath);
bool TryLoadMesh(std::wstring szPath);

// Game object component
struct MeshRenderer
{
	std::wstring szMeshPath;
	Vector3 offset;
	Vector3 rotation;
	Vector3 scaling;
	std::wstring szVertexShader;
	std::wstring szPixelShader;
};


