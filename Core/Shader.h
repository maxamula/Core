#pragma once
#include "Resource.h"
#include <d3d11.h>

class Shader : public Resource
{
public:
	ID3DBlob* GetShader();
protected:
	void LoadResource(std::wstring filepath) override;
	void UnloadResource() override;

	ID3DBlob* pBytecode = NULL;
};

