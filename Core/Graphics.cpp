#include "Graphics.h"
#include "Window.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Shader.h"
#include <d3dcompiler.h>

extern Window window;
extern ResourceManager resources;

namespace wrl = Microsoft::WRL;

//	Create d3d interfaces, depth buffer and set viewport
void Graphics::CreateInterfaces(HWND hWnd, Resolution resolution)
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

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	HRESULT zalupa = pDev->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pTarg.GetAddressOf(), pDSV.Get());

	D3D11_VIEWPORT vp;
	vp.Width = window.width;
	vp.Height = window.height;
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

void Graphics::DrawScene(Scene* pScene)
{
	for (auto [object, mesh, trans] : pScene->registry.view<MeshComponent, Transformation>().each())
	{
		// SetUp vertex/index buffer
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = mesh.mesh.nVertices;
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = mesh.mesh.pVertexData;
		pDev->CreateBuffer(&bd, &sd, &pVertexBuffer);
		// Bind Vertex Buffer
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
		// IndexBuffer
		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = mesh.mesh.nIndices;
		ibd.StructureByteStride = sizeof(unsigned int);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = mesh.mesh.pIndexData;
		pDev->CreateBuffer(&ibd, &isd, &pIndexBuffer);
		// Bind Index Buffer
		pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

		// UPDATE MATRIX
		const TransCBuf cb =
		{
			{
				DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationRollPitchYaw(mesh.rotation.x + trans.rotation.x, mesh.rotation.y + trans.rotation.y, mesh.rotation.z + trans.rotation.z) *
				DirectX::XMMatrixScaling(mesh.scale.x + trans.scaling.x, mesh.scale.y + trans.scaling.y, mesh.scale.z + trans.scaling.z) *
				DirectX::XMMatrixTranslation(mesh.offset.x + trans.position.x, mesh.offset.y + trans.position.y, mesh.offset.z + trans.position.z) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.5f, 80.0f)
				)
			}
		};
		UpdateMatrix(cb);

		struct ColorCBuf
		{
			float r, g, b, a;
		};
		::Color col = {1.0f, 1.0f, 1.0f};
		if(mesh.material.Has<::Color>())
			col = mesh.material.Get<::Color>();
		const ColorCBuf colorbuf = {col.color.x, col.color.y, col.color.z, 1.0f};
		wrl::ComPtr<ID3D11Buffer> pColorBuffer;
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.CPUAccessFlags = 0u;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(colorbuf);
		cbd.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &colorbuf;
		pDev->CreateBuffer(&cbd, &csd, &pColorBuffer);
		// bind pixel constant buffer
		pContext->PSSetConstantBuffers(0u, 1u, pColorBuffer.GetAddressOf());
		
		ID3DBlob* PSBytecode = NULL;
		ID3DBlob* VSBytecode = NULL;
		// todo: load shaders based on material params

		// Pixel Shader
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		PSBytecode = resources.Get<Shader>(L"C:\\DefaultPS.cso")->GetShader();
		pDev->CreatePixelShader(PSBytecode->GetBufferPointer(), PSBytecode->GetBufferSize(), nullptr, &pPixelShader);
		// Bind Pixel Shader
		pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		// Vertex Shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		VSBytecode = resources.Get<Shader>(L"C:\\DefaultVS.cso")->GetShader();
		pDev->CreateVertexShader(VSBytecode->GetBufferPointer(), VSBytecode->GetBufferSize(), nullptr, &pVertexShader);
		// Bind Vertex Shader
		pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

		// Input Layout
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
			//{ "Normal", 0,DXGI_FORMAT_R32G32B32_FLOAT,0,12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		pDev->CreateInputLayout(ied, (UINT)std::size(ied), VSBytecode->GetBufferPointer(), VSBytecode->GetBufferSize(), &pInputLayout);
		pContext->IASetInputLayout(pInputLayout.Get());

		// Primitive Topology
		pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		pContext->DrawIndexed((UINT)36u, 0u, 0u);
	}
}

// Update vertex shader const buffer
void Graphics::UpdateMatrix(const TransCBuf& cbuf)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	pContext->Map(pTransformBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy(msr.pData, &cbuf, sizeof(TransCBuf));
	pContext->Unmap(pTransformBuffer.Get(), 0u);
}

void Graphics::BeginFrame()
{
	pContext->ClearRenderTargetView(pTarg.Get(), (const float*)&voidColor);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}