#pragma once
#include "Mesh.h"

class Primitives
{
public:
	static Mesh Cube();

	static void InitializePrimitivesModels();
	static void ReleasePrimitivesModels();
private:
	

	static Vertex* pCubeVertexData;
	static unsigned int* pCubeIndexData;
};
