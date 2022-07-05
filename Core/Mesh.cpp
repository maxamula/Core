#include "Mesh.h"
#include "GameObject.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace wrl = Microsoft::WRL;

void Scene::DrawScene()
{
	for (auto [object, pos, mesh, renderer] : registry.view<PositionComponent, Mesh, MeshRenderer>().each())	// for each object on the scene
	{
		// Vertex buffer
		wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = mesh.vertices;
		bd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = mesh.pVertexData;
		m_core->gfx().pDev->CreateBuffer(&bd, &sd, &pVertexBuffer);
		// Bind Vertex Buffer
		const UINT stride = sizeof(Vertex);
		const UINT offset = 0u;
		m_core->gfx().pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

		// Index Buffer
		wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = mesh.indices;
		ibd.StructureByteStride = sizeof(unsigned int);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = mesh.pIndexData;
		m_core->gfx().pDev->CreateBuffer(&ibd, &isd, &pIndexBuffer);
		// Bind Index Buffer
		m_core->gfx().pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0u);


		// Update transformation matrix
		const TransCBuf cb =
		{
			{
				m_activeCamera.GetMatrix({pos.x + renderer.offset.x, pos.y + renderer.offset.y, pos.z + renderer.offset.z }, renderer.scaling, renderer.rotation, 9.0f/16.0f)
			}
		};
		m_core->gfx().UpdateMatrix(cb);

		// cube colors
		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{1.0f,0.0f,1.0f},
				{1.0f,0.0f,0.0f},
				{0.0f,1.0f,0.0f},
				{0.0f,0.0f,1.0f},
				{1.0f,1.0f,0.0f},
				{0.0f,1.0f,1.0f},
			}
		};
		wrl::ComPtr<ID3D11Buffer> pConstantBuffer2;
		D3D11_BUFFER_DESC cbd2;
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.Usage = D3D11_USAGE_DEFAULT;
		cbd2.CPUAccessFlags = 0u;
		cbd2.MiscFlags = 0u;
		cbd2.ByteWidth = sizeof(cb2);
		cbd2.StructureByteStride = 0u;
		D3D11_SUBRESOURCE_DATA csd2 = {};
		csd2.pSysMem = &cb2;
		m_core->gfx().pDev->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);
		// bind pixel constant buffer
		m_core->gfx().pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

		// Pixel Shader
		wrl::ComPtr<ID3D11PixelShader> pPixelShader;
		ID3DBlob* pBlob = m_core->resources().GetShader(L"C:\\PixelShader.cso");
		m_core->gfx().pDev->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
		// Bind Pixel Shader
		m_core->gfx().pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

		// Vertex Shader
		wrl::ComPtr<ID3D11VertexShader> pVertexShader;
		pBlob = m_core->resources().GetShader(L"C:\\VertexShader.cso");
		m_core->gfx().pDev->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
		// Bind Vertex Shader
		m_core->gfx().pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


		// Input Layout
		wrl::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		m_core->gfx().pDev->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
		m_core->gfx().pContext->IASetInputLayout(pInputLayout.Get());


		// Primitive Topology
		m_core->gfx().pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_core->gfx().pContext->DrawIndexed((UINT)36u, 0u, 0u);
	}
}