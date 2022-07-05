#pragma once
#include "Window.h"
#include "Core.h"
#include "Camera.h"
#include "GameObject.h"
#include <wrl/client.h>
#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

class Core;
class Scene;

struct TransCBuf
{
	DirectX::XMMATRIX transform;
};

class Graphics
{
	friend class Core;
	friend class Window;
	friend class Scene;
public:	
	struct Resolution
	{
		USHORT resX, resY;
	};

	struct Color {
		float r, g, b;
	};

public:
	Graphics(Core* parent);
	void SetResolution(Resolution resolution);
	void SetVoidColor(float r, float g, float b);
	Resolution GetResoution() const;
	Color GetVoidColor() const;
	
private:
	void CreateInterfaces(Resolution resolution, HWND hWnd);
	void CreateInterfaces(HWND hWnd);
	void UpdateMatrix(const TransCBuf& cbuf);
	void BeginFrame();
	void EndFrame();
private:
	Core* m_core = NULL;

	std::unique_ptr<Window> wnd;
	Resolution m_resolution = { 1280, 720 };
	float m_renderDist = 30.0f;
	Color m_voidColor = {0.1f, 0.1f, 0.3f};

	Microsoft::WRL::ComPtr<ID3D11Device> pDev;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarg;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	
	Microsoft::WRL::ComPtr<ID3D11Buffer> pTransformBuffer;
};

