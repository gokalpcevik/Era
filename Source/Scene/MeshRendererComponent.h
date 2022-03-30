#pragma once
#include <d3d11_3.h>
#include "../Asset/MeshAsset.h"
#include "../Renderer/ConstantBuffer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/ShaderLibrary.h"
#include "../Renderer/InputLayout.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/SamplerState.h"
#include "../Renderer/Material.h"
#include "../Renderer/Layout.h"

namespace Era
{
	class MeshRendererComponent
	{
		struct Vertex;
		struct VSConstantBufferData;
		struct PSConstantBufferData;
	public:
		MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset, const MaterialRef& material);

		[[nodiscard]] auto GetVertexBuffer() const -> const VertexBufferRef<Vertex>& { return m_VertexBuffer; }
		[[nodiscard]] auto GetVSConstantBuffer() const -> const ConstantBufferRef<VSConstantBufferData>& { return m_VSConstantBuffer; }
		[[nodiscard]] auto GetIndexBuffer() const -> const IndexBufferRef& { return m_IndexBuffer; }
		[[nodiscard]] auto GetMaterial() const -> const MaterialRef& { return m_Material; }

	private:
		void SetWorldViewProjectionMatrices(ID3D11DeviceContext* pContext, const DX::XMMATRIX& WorldViewProjection);
		void SetWorldMatrix(ID3D11DeviceContext* pContext, const DX::XMMATRIX& World);
		void CreateVertexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset);
		void CreateIndexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset);
		void CreateVertexShaderConstantBuffer(ID3D11Device* pDevice);

		friend class Scene;
	public:
		struct Vertex
		{
			Vertex(DX::XMFLOAT3 Position, DX::XMFLOAT3 Normal, DX::XMFLOAT2 UV) : Position(Position), Normal(Normal), UV(UV) {}
			Vertex(DX::XMFLOAT3 Position, DX::XMFLOAT3 Normal, DX::XMFLOAT2 UV, uint32_t UseSamplers) : Position(Position), Normal(Normal), UV(UV) ,UseSamplers(UseSamplers){}

			DX::XMFLOAT3 Position{};
			DX::XMFLOAT3 Normal{};
			DX::XMFLOAT2 UV{};
			uint32_t UseSamplers = 1;
		};

		struct VSConstantBufferData
		{
			DX::XMMATRIX World{};
			DX::XMMATRIX WorldViewProjection{};
			DX::XMFLOAT4 Color{ 1.0f,1.0f,1.0f,1.0f };
		};

	private:
		VertexBufferRef<Vertex> m_VertexBuffer{};
		IndexBufferRef m_IndexBuffer{};
		ConstantBufferRef<VSConstantBufferData> m_VSConstantBuffer{};
		VSConstantBufferData m_VSConstantBufferData{};
		MaterialRef m_Material{};
	};
}