#include "Graphics.h"

namespace wrl = Microsoft::WRL;

Graphics::Graphics(Core* core) : m_core(core)
{
	wnd = std::make_unique<Window>(m_core);
}

void Graphics::CreateInterfaces(Resolution resolution, HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC desc = { };
	desc.BufferDesc.Width = resolution.resX;
	desc.BufferDesc.Height = resolution.resY;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &desc, &pSwap, &pDev, NULL, &pContext);
	ID3D11Resource* pBackBuf = NULL;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuf);
	pDev->CreateRenderTargetView(pBackBuf, NULL, &pTarg);
	pBackBuf->Release();

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDev->CreateDepthStencilState(&dsDesc, &pDSState);

	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = resolution.resX;
	descDepth.Height = resolution.resY;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDev->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	//// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	HRESULT zalupa = pDev->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarg.GetAddressOf(), pDSV.Get());

	D3D11_VIEWPORT vp;
	vp.Width = resolution.resX;
	vp.Height = resolution.resY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1, &vp);

	// Create constant buffer that will contain transformation matrix
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(TransCBuf);
	cbd.StructureByteStride = 0u;
	pDev->CreateBuffer(&cbd, nullptr, &pTransformBuffer);
	pContext->VSSetConstantBuffers(0u, 1u, pTransformBuffer.GetAddressOf());
}

void Graphics::CreateInterfaces(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC desc = { };
	desc.BufferDesc.Width = m_resolution.resX;
	desc.BufferDesc.Height = m_resolution.resY;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount = 1;
	desc.OutputWindow = hWnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &desc, &pSwap, &pDev, NULL, &pContext);
	ID3D11Resource* pBackBuf = NULL;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuf);
	pDev->CreateRenderTargetView(pBackBuf, NULL, &pTarg);
	pBackBuf->Release();

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDev->CreateDepthStencilState(&dsDesc, &pDSState);

	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_resolution.resX;
	descDepth.Height = m_resolution.resY;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDev->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	//// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	HRESULT zalupa = pDev->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarg.GetAddressOf(), pDSV.Get());

	D3D11_VIEWPORT vp;
	vp.Width = m_resolution.resX;
	vp.Height = m_resolution.resY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pContext->RSSetViewports(1, &vp);

	// Create constant buffer that will contain transformation matrix
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(TransCBuf);
	cbd.StructureByteStride = 0u;
	pDev->CreateBuffer(&cbd, nullptr, &pTransformBuffer);
	pContext->VSSetConstantBuffers(0u, 1u, pTransformBuffer.GetAddressOf());
}

void Graphics::SetResolution(Resolution resolution)
{
	m_resolution = resolution;
	if (wnd->GetWindowState())
	{
		CreateInterfaces(m_resolution, wnd->WinId());
	}
}

void Graphics::SetVoidColor(float r, float g, float b)
{
	m_voidColor.r = r;
	m_voidColor.g = g;
	m_voidColor.b = b;
}

Graphics::Resolution Graphics::GetResoution() const
{
	return m_resolution;
}

Graphics::Color Graphics::GetVoidColor() const
{
	return m_voidColor;
}


void Graphics::UpdateMatrix(const TransCBuf& cbuf)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	pContext->Map(pTransformBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy(msr.pData, &cbuf, sizeof(TransCBuf));
	pContext->Unmap(pTransformBuffer.Get(), 0u);
}

void Graphics::BeginFrame()
{
	pContext->ClearRenderTargetView(pTarg.Get(), (const float*)&m_voidColor);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}