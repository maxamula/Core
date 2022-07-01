#pragma once
#include "GameObject.h"
#include "Mesh.h"


GameObject Cube(Scene* pScene, std::wstring tag)
{
	auto object = pScene->CreateEntity(tag);
	Mesh cuboidMesh;
	Vertex* pVertData = (Vertex*)malloc(8*sizeof(Vertex));
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f	 },
		{ 1.0f,-1.0f,-1.0f	 },
		{ -1.0f,1.0f,-1.0f	 },
		{ 1.0f,1.0f,-1.0f	 },
		{ -1.0f,-1.0f,1.0f	 },
		{ 1.0f,-1.0f,1.0f	 },
		{ -1.0f,1.0f,1.0f	 },
		{ 1.0f,1.0f,1.0f	 },
	};
	memcpy(pVertData, vertices, 8 * sizeof(Vertex));
	cuboidMesh.pVertexData = pVertData;
	cuboidMesh.vertices = 8*sizeof(Vertex);

	unsigned int* pIndexData = (unsigned int*)malloc(36*sizeof(unsigned int));
	const unsigned int indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	memcpy(pIndexData, indices, 36 * sizeof(unsigned int));
	cuboidMesh.pIndexData = pIndexData;
	cuboidMesh.indices = 36*sizeof(unsigned int);

	MeshRenderer renderer;
	renderer.offset = { 0.0f, 0.0f, 0.0f };
	renderer.rotation = { 0.0f, 0.0f, 0.0f };
	renderer.scaling = { 1.0f, 1.0f, 1.0f };
	renderer.szMeshPath = L"EXAMPLE";
	renderer.szPixelShader = L"EXAMPLE";
	renderer.szVertexShader = L"EXAMPLE";

	object.AddComponent<Mesh>(cuboidMesh);
	object.AddComponent<MeshRenderer>(renderer);
	return object;
}

