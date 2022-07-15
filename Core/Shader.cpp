#include "Shader.h"
#include <d3dcompiler.h>

ID3DBlob* Shader::GetShader()
{
	return pBytecode;
}

void Shader::LoadResource(std::wstring filepath)
{
	D3DReadFileToBlob(filepath.c_str(), &pBytecode);
}

void Shader::UnloadResource()
{
	if (pBytecode != NULL)
	{
		pBytecode->Release();
		pBytecode = NULL;
	}
}