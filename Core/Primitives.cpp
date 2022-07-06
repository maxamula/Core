#include "Primitives.h"

Vertex* Primitives::pCubeVertexData;
unsigned int* Primitives::pCubeIndexData;
bool Primitives::bInitialized = false;
int Primitives::nRefs = 0;

GameObject Primitives::Cube(Scene* pScene, std::wstring tag)
{
	auto object = pScene->CreateObject(tag);
	Mesh cuboidMesh = { pCubeVertexData, 8 * sizeof(Vertex), pCubeIndexData, 36 * sizeof(unsigned int) };
	MeshRenderer renderer = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, L"EXAMPLE", L"EXAMPLE" };
	object.AddComponent<Mesh>(cuboidMesh);
	object.AddComponent<MeshRenderer>(renderer);
	return object;
}

void Primitives::InitializePrimitivesModels()
{
	if (bInitialized)
		return;
	// Cube
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f},
		{ 1.0f,-1.0f,-1.0f},
		{ -1.0f,1.0f,-1.0f},
		{ 1.0f,1.0f,-1.0f},
		{ -1.0f,-1.0f,1.0f},
		{ 1.0f,-1.0f,1.0f},
		{ -1.0f,1.0f,1.0f},
		{ 1.0f,1.0f,1.0f},
	};
	pCubeVertexData = (Vertex*)malloc(8 * sizeof(Vertex));
	memcpy(pCubeVertexData, vertices, 8 * sizeof(Vertex));
	const unsigned int indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	pCubeIndexData = (unsigned int*)malloc(36 * sizeof(unsigned int));
	memcpy(pCubeIndexData, indices, 36 * sizeof(unsigned int));

	nRefs++;
	bInitialized = true;
}

void Primitives::ReleasePrimitivesModels()
{
	if (!bInitialized)
		return;
	nRefs--;
	if (nRefs != 0)
		return;
	// Cube
	free(pCubeVertexData);
	free(pCubeIndexData);
}