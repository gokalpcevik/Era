#pragma once
#include "Shader.h"
#include "ShaderLibrary.h"
#include "TextureArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "SamplerState.h"
#include <filesystem>
#include <DirectXMath.h>

namespace Era
{
	enum class CullMode
	{
		CullBack,
		CullFront,
		CullNone
	};

	enum class FillMode
	{
		FillWireframe,
		FillSolid
	};

	namespace DX = DirectX;

	class Material
	{
	public:
		Material(std::filesystem::path vertexShaderPath, std::filesystem::path pixelShaderPath);

		void SetDiffuse(const Texture2DRef& diffuse);
		void SetSpecular(const Texture2DRef& specular);
		void SetCullMode(CullMode mode) { m_CullMode = mode; }
		void SetFillMode(FillMode mode) { m_FillMode = mode; }

		[[nodiscard]] auto GetSpecular() const -> const Texture2DRef& { return m_Specular; }
		[[nodiscard]] auto GetDiffuse() const -> const Texture2DRef& { return m_Diffuse; }
		[[nodiscard]] auto GetCullMode() const -> CullMode { return m_CullMode; }
	private:
		std::filesystem::path m_VertexShaderPath{};
		std::filesystem::path m_PixelShaderPath{};
		Texture2DRef m_Diffuse{};
		Texture2DRef m_Specular{};
		VertexShaderRef m_VertexShader{};
		PixelShaderRef m_PixelShader{};
		CullMode m_CullMode = CullMode::CullBack;
		FillMode m_FillMode = FillMode::FillSolid;
		ShaderBlob m_VSBlob{nullptr,nullptr};
		ShaderBlob m_PSBlob{ nullptr,nullptr };
	};

	using MaterialRef = std::shared_ptr<Material>;
}