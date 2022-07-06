#pragma once
#include "GameObject.h"
#include "Mesh.h"

class Primitives
{
	friend class ResourceManager;
public:
	static GameObject Cube(Scene* pScene, std::wstring tag);
private:
	static void InitializePrimitivesModels();
	static void ReleasePrimitivesModels();

	static bool bInitialized;
	static int nRefs;
	static Vertex* pCubeVertexData;
	static unsigned int* pCubeIndexData;
};

