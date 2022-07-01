#include "ObjectRenderer.h"

namespace wrl = Microsoft::WRL;

ObjectRenderer::ObjectRenderer(Graphics* gfx) : pGfx(gfx)
{
	
}

bool ObjectRenderer::RenderObject(DrawableObject* pObject)
{
	// Vertex buffer
	auto vertices = pObject->GetVetices();
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	pGfx->pDev->CreateBuffer(&bd, &sd, &pVertexBuffer);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pGfx->pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);



	// Index buffer
	auto indices = pObject->GetIndices();
	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = (UINT)(sizeof(unsigned int) * indices.size());
	ibd.StructureByteStride = sizeof(unsigned int);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();
	pGfx->pDev->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	pGfx->pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);

	// Transformation stuff
	transformation =
	{
		DirectX::XMMatrixTranspose
		(
		DirectX::XMMatrixScaling(pObject->scaling.x, pObject->scaling.y, pObject->scaling.z) *
		DirectX::XMMatrixRotationX(pObject->rotation.x) *
		DirectX::XMMatrixRotationY(pObject->rotation.y) *
		DirectX::XMMatrixRotationZ(pObject->rotation.z) *
		DirectX::XMMatrixTranslation(pObject->pos.x, pObject->pos.y, pObject->pos.z) *
		DirectX::XMMatrixPerspectiveLH(1.0f, pGfx->GetResoution().resX / pGfx->GetResoution().resY, 0.5f, 30.0f)
		)
	};
	UpateTransBuf();

	// Shaders
	// Pixel Shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	if (D3DReadFileToBlob(L"C:\\PixelShader.cso", &pBlob) != S_OK)
		return false;
	if (pGfx->pDev->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader))
		return false;
	pGfx->pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	// Vertex Shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	if (D3DReadFileToBlob(L"C:\\VertexShader.cso", &pBlob) != S_OK)
		return false;
	if (pGfx->pDev->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader) != S_OK)
		return false;
	pGfx->pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	
	// Input Layout
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	pGfx->pDev->CreateInputLayout(&inputLayout, 1u, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	pGfx->pContext->IASetInputLayout(pInputLayout.Get());

	// Topology
	pGfx->pContext->IASetPrimitiveTopology(pObject->GetDesc().topology);

}

void ObjectRenderer::Init()
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(TransCBuf);
	cbd.StructureByteStride = 0u;
	pGfx->pDev->CreateBuffer(&cbd, nullptr, &pTranformBuffer);
}

void ObjectRenderer::UpateTransBuf()
{
	D3D11_MAPPED_SUBRESOURCE msr;
	pGfx->pContext->Map(pTranformBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy(msr.pData, &transformation, sizeof(transformation));
	pGfx->pContext->Unmap(pTranformBuffer.Get(), 0u);
}
