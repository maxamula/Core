#include "Primitives.h"

Vertex* Primitives::pCubeVertexData;
unsigned int* Primitives::pCubeIndexData;

Mesh Primitives::Cube()
{
	Mesh cubeMesh = { pCubeVertexData, 8 * sizeof(Vertex), pCubeIndexData, 36 * sizeof(unsigned int) };
	return cubeMesh;
}

void Primitives::InitializePrimitivesModels()
{
	// Cube
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f },
		{ 1.0f,-1.0f,-1.0f },
		{ -1.0f,1.0f,-1.0f },
		{ 1.0f,1.0f,-1.0f },
		{ -1.0f,-1.0f,1.0f },
		{ 1.0f,-1.0f,1.0f, },
		{ -1.0f,1.0f,1.0f, },
		{ 1.0f,1.0f,1.0f, },
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
}

void Primitives::ReleasePrimitivesModels()
{
	// Cube
	free(pCubeVertexData);
	free(pCubeIndexData);
}