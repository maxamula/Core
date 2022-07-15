#pragma once
#include "Vector.h"
#include "GameObject.h"
#include "Material.h"

struct Vertex
{
	float x, y, z;	// coordinates
};

struct Mesh
{
	Vertex* pVertexData = NULL;				// pointer to vertices array
	uint32_t nVertices = 0;					// number of vertices in the array
	uint32_t* pIndexData = NULL;			// pointer to array of indices
	uint32_t nIndices = 0;					// number of indices
};

// GameObject mesh component
struct MeshComponent
{
	vec3 offset = { 0.0f, 0.0f, 0.0f };						// mesh offset from gameobject position
	vec3 rotation = { 0.0f, 0.0f, 0.0f };					// mesh rortation
	vec3 scale = { 1.0f, 1.0f, 1.0f };						// mesh scaling
	Material material = Material::GetMaterial("Default");	// mesh material
	Mesh mesh;												// mesh data
};