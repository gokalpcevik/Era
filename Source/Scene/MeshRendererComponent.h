#pragma once
#include <d3d11.h>
#include "../Asset/MeshAsset.h"
#include "../Renderer/ConstantBuffer.h"
#include "../Renderer/VertexBuffer.h"
#include "../Renderer/IndexBuffer.h"
#include "../Renderer/Shader.h"
#include "../Renderer/ShaderLibrary.h"
#include "../Renderer/InputLayout.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/SamplerState.h"

namespace Era
{
	class MeshRendererComponent
	{
		struct Vertex;
		struct VSConstantBufferData;
		struct PSConstantBufferData;
	public:
		MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset);
		MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset, const std::filesystem::path& texturePath);

		[[nodiscard]] auto GetVertexBuffer() const -> const VertexBufferRef<Vertex>& { return m_VertexBuffer; }
		[[nodiscard]] auto GetVSConstantBuffer() const -> const ConstantBufferRef<VSConstantBufferData>& { return m_VSConstantBuffer; }
		[[nodiscard]] auto GetPSConstantBuffer() const -> const ConstantBufferRef<PSConstantBufferData>& { return m_PSConstantBuffer; }
		[[nodiscard]] auto GetIndexBuffer() const -> const IndexBufferRef& { return m_IndexBuffer; }
		[[nodiscard]] auto GetVertexShader() const -> const VertexShaderRef& { return m_VertexShader; }
		[[nodiscard]] auto GetPixelShader() const -> const PixelShaderRef& { return m_PixelShader; }
		[[nodiscard]] auto GetSamplerState() const -> const SamplerStateRef& { return m_SamplerState; }
		[[nodiscard]] auto GetTexture2D() const -> const Texture2DRef& { return m_Texture2DRef; }
		[[nodiscard]] auto GetInputLayout() const -> const InputLayoutRef& { return m_InputLayout; }

		void SetWorldViewProjection(ID3D11DeviceContext* pContext, const DX::XMMATRIX& WorldViewProjection);
		void SetWorld(ID3D11DeviceContext* pContext, const DX::XMMATRIX& World);
		void UpdateLightData(ID3D11DeviceContext* pContext, const PSConstantBufferData& data);
	private:
		void CreateVertexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset);
		void CreateIndexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset);
		void CreateVertexShader(ID3D11Device* pDevice);
		void CreatePixelShader(ID3D11Device* pDevice);
		void CreateInputLayout(ID3D11Device* pDevice, const VertexShaderRef& vs);
		void CreateVertexShaderConstantBuffer(ID3D11Device* pDevice);
		void CreatePixelShaderConstantBuffer(ID3D11Device* pDevice);
	public:
		struct Vertex
		{
			Vertex(DX::XMFLOAT3 Position, DX::XMFLOAT3 Normal, DX::XMFLOAT2 UV) : Position(Position), Normal(Normal), UV(UV) {}

			DX::XMFLOAT3 Position{};
			DX::XMFLOAT3 Normal{};
			DX::XMFLOAT2 UV{};
			bool UseSamplers = false;
		};

		struct VSConstantBufferData
		{
			DX::XMMATRIX World{};
			DX::XMMATRIX WorldViewProjection{};
			DX::XMFLOAT4 Color{ 1.0f,1.0f,1.0f,1.0f };
		};

		struct PSConstantBufferData
		{
			alignas(16) DX::XMFLOAT4 CameraPosition{0.0f,0.0f,0.0f,1.0f};
			alignas(16) DX::XMFLOAT4 LightDirection{ 0.0f,0.0f,1.0f,1.0f }; //16
			alignas(16) DX::XMFLOAT4 AmbientLightColor{ 1.0f,1.0f,1.0f,1.0f }; //12
			alignas(16) DX::XMFLOAT4 DiffuseLightColor{ 1.0f,1.0f,1.0f ,1.0f};; //12
			alignas(16) DX::XMFLOAT4 SpecularLightColor{ 1.0f,1.0f,1.0f ,1.0f};; //12
			float AmbientCoefficient = 0.08f; //4
			float DiffuseCoefficient = 0.6f; //4
			float SpecularCoefficient = 1.0f; //4
			float Shininess = 400.0f;
		};

	private:
		VertexBufferRef<Vertex> m_VertexBuffer{};
		IndexBufferRef m_IndexBuffer{};
		VertexShaderRef m_VertexShader{};
		InputLayoutRef m_InputLayout{};
		PixelShaderRef m_PixelShader{};
		Texture2DRef m_Texture2DRef{};
		SamplerStateRef m_SamplerState{};
		ConstantBufferRef<VSConstantBufferData> m_VSConstantBuffer{};
		ConstantBufferRef<PSConstantBufferData> m_PSConstantBuffer{};
		VSConstantBufferData m_VSConstantBufferData{};
		PSConstantBufferData m_PSConstantBufferData{};
	};
}