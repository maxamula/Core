#pragma once
#include "GameObject.h"
#include <wrl/client.h>
#include <d3d11.h>
#include <DirectXMath.h>

// Transformation matrix constant buffer that will be updating for every object
struct TransCBuf
{
	DirectX::XMMATRIX transform;
};

class Graphics
{
public:
	struct Resolution
	{
		USHORT resX, resY;
	};

	struct Color {
		float r, g, b;
	};

public:
	// Create d3d interfaces
	void CreateInterfaces(HWND hWnd, Resolution resolution = {1280, 720});
	// Main Drawing method
	void DrawScene(Scene* pScene);
	// update viewmatrix
	void UpdateMatrix(const TransCBuf& cbuf);
	void BeginFrame();
	void EndFrame();

	Microsoft::WRL::ComPtr<ID3D11Device> pDev;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarg;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	Color voidColor = { 0.0f, 0.0f, 0.0f };
private:


	Microsoft::WRL::ComPtr<ID3D11Buffer> pTransformBuffer;
};
