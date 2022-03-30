#pragma once
#include "Shader.h"
#include "ShaderLibrary.h"
#include "TextureArray.h"
#include "ConstantBuffer.h"
#include "InputLayout.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include <filesystem>
#include <DirectXMath.h>

namespace Era
{
	enum class CullMode
	{
		CullBack  = 3,
		CullFront = 2,
		CullNone  = 1
	};

	enum class FillMode
	{
		FillWireframe = 2,
		FillSolid	  = 3
	};

	namespace DX = DirectX;

	struct alignas(16) PSDefaultCBuffer
	{
		DX::XMFLOAT4 CameraPosition{ 0.0f,0.0f,0.0f,1.0f };
		DX::XMFLOAT4 DiffuseLightColor{ 1.0f,1.0f,1.0f ,1.0f };
		DX::XMFLOAT4 SpecularLightColor{ 1.0f,1.0f,1.0f ,1.0f };
		DX::XMFLOAT3 LightDirection{ 0.0f,0.0f,1.0f};
		float Intensity = 10.0f;
		float Roughness = 0.5f;
		float Metallic = 0.04f;
		float AO = 0.04f;
	};

	class Material
	{
		friend class Scene;
		friend class Renderer;
	public:
		Material(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::filesystem::path vertexShaderPath,
		         std::filesystem::path pixelShaderPath);

		void SetCullMode(CullMode mode);
		void SetFillMode(FillMode mode);
		void SetRoughness(float r);
		void SetMetallic(float m);
		void SetAO(float ao);
		void PushTexture(const Texture2DRef& texture) const;
		void SetSamplerState(const SamplerStateRef& samplerState) ;
		void RemoveTexture(const size_t index) const;

		[[nodiscard]] auto GetCullMode() const -> CullMode { return m_CullMode; }
		[[nodiscard]] auto GetRoughness() const -> float { return m_DefaultCBData.Roughness; }
		[[nodiscard]] auto GetMetallic() const -> float { return m_DefaultCBData.Metallic; }
		[[nodiscard]] auto GetAO() const -> float { return m_DefaultCBData.AO; }
		auto GetTexture(size_t index) const -> const Texture2DRef&;
		/*
		 * @data Data to be set.
		 * @index Determines which buffer should the data be set.
		 */
		void SetData(ID3D11DeviceContext* pContext,void* data,size_t index) const;
	private:
		static auto GetNumberOfConstantBuffers(ID3DBlob* blob) -> size_t;
		void UpdateLightData(ID3D11DeviceContext* pContext, const PSDefaultCBuffer& data) const;
		void Bind() const;
		void UnbindTextures();
	private:
		ID3D11Device* m_Device{ nullptr };
		ID3D11DeviceContext* m_DeviceCtx{ nullptr };
		std::filesystem::path m_VertexShaderPath{};
		std::filesystem::path m_PixelShaderPath{};
		TextureArrayRef m_Textures{};
		SamplerStateRef m_SamplerState{};
		VertexShaderRef m_VertexShader{};
		InputLayoutRef m_InputLayout{};
		PixelShaderRef m_PixelShader{};
		ConstantBufferRef<PSDefaultCBuffer> m_DefaultConstantBuffer{};
		std::vector<std::shared_ptr<DynamicConstantBuffer>> m_ConstantBuffers;
		RasterizerStateRef m_RasterizerState{};
		CullMode m_CullMode = CullMode::CullBack;
		FillMode m_FillMode = FillMode::FillSolid;
		ShaderBlob m_VSBlob{nullptr,nullptr};
		ShaderBlob m_PSBlob{ nullptr,nullptr };
		D3D11_RASTERIZER_DESC m_RasterizerDesc{};
		PSDefaultCBuffer m_DefaultCBData{};
	};

	using MaterialRef = std::shared_ptr<Material>;
}
