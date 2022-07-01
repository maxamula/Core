#pragma once
#include "Graphics.h"
#include "DrawableObject.h"
#include <wrl/client.h>
#include <memory>
#include <DirectXMath.h>

class Graphics;
class DrawableObject;

struct PIPELINE_DESC
{
	D3D11_PRIMITIVE_TOPOLOGY topology;
	const WCHAR* szVertexShader;
	const WCHAR* szPixelShader;
};

class ObjectRenderer
{
public:
	struct TransCBuf
	{
		DirectX::XMMATRIX transformation;
	};
public:
	ObjectRenderer(Graphics* gfx);
	void Init();
	bool RenderObject(DrawableObject* pObject);
private:
	void UpateTransBuf();

	Graphics* pGfx = NULL;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pTranformBuffer;
	TransCBuf transformation;
	D3D11_INPUT_ELEMENT_DESC inputLayout = { "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	bool bInit = false;
};

