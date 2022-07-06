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

// Game object component
struct MeshRenderer
{
	Vector3 offset;
	Vector3 rotation;
	Vector3 scaling;
	std::wstring szVertexShader;
	std::wstring szPixelShader;
};


