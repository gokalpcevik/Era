#include "MeshRendererComponent.h"

namespace Era
{
	
	MeshRendererComponent::MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset,
		const MaterialRef& material) : m_Material(material)
	{
		CreateIndexBuffer(pDevice, meshAsset);
		CreateVertexBuffer(pDevice, meshAsset);
		CreateVertexShaderConstantBuffer(pDevice);

	}

	void MeshRendererComponent::SetWorldViewProjectionMatrices(ID3D11DeviceContext* pContext,
	                                                   const DX::XMMATRIX& WorldViewProjection)
	{
		m_VSConstantBufferData.WorldViewProjection = WorldViewProjection;
		m_VSConstantBuffer->Update(pContext, m_VSConstantBufferData);
	}

	void MeshRendererComponent::SetWorldMatrix(ID3D11DeviceContext* pContext, const DX::XMMATRIX& World)
	{
		m_VSConstantBufferData.World = World;
		m_VSConstantBuffer->Update(pContext, m_VSConstantBufferData);
	}

	void MeshRendererComponent::CreateVertexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset)
	{
		std::vector<Vertex> vertices{};
		vertices.reserve(meshAsset.GetNumVertices());
		for (size_t i = 0; i < meshAsset.GetNumVertices(); ++i)
		{
			DX::XMFLOAT3 Pos{ meshAsset.GetVertexPositions()[i].x ,meshAsset.GetVertexPositions()[i].y ,meshAsset.GetVertexPositions()[i].z };
			DX::XMFLOAT3 Normal{ meshAsset.GetVertexNormals()[i].x,meshAsset.GetVertexNormals()[i].y,meshAsset.GetVertexNormals()[i].z };
			vertices.emplace_back(Pos, Normal, meshAsset.GetTextureCoordinates(0u)[i],0);
		}
		m_VertexBuffer = std::make_shared<VertexBuffer<Vertex>>(pDevice, vertices.data(), vertices.size());
	}

	void MeshRendererComponent::CreateIndexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset)
	{
		m_IndexBuffer = std::make_shared<IndexBuffer>(pDevice, meshAsset.GetIndices().data(), meshAsset.GetNumIndices());
	}

	void MeshRendererComponent::CreateVertexShaderConstantBuffer(ID3D11Device* pDevice)
	{
		m_VSConstantBuffer = std::make_shared<ConstantBuffer<VSConstantBufferData>>(pDevice, m_VSConstantBufferData, ConstantBufferType::Vertex);
	}
}
