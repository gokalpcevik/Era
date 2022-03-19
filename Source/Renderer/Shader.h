#pragma once

#include <d3d11_3.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string_view>
#include <filesystem>
#include <random>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	enum class ShaderType
	{
		Unknown = -1,
		Vertex = 0,
		Pixel,
		Compute,
		Geometry,
	};

	struct ShaderBlob
	{
		ShaderBlob() = default;
		auto operator==(const ShaderBlob& other) const -> bool
		{
			return Blob == other.Blob && Blob == other.ErrorBlob;
		}

		ID3DBlob* Blob{ nullptr };
		ID3DBlob* ErrorBlob{ nullptr };
		ShaderType Type{};
	};

	struct ShaderBlobHashFn
	{
		auto operator() (const std::filesystem::path& path) const -> std::size_t
		{
			return std::filesystem::hash_value(path);
		}
	};

	auto GetShaderTypeFromExtension(const std::filesystem::path& extension) -> ShaderType;
	auto GetShaderTargetFromType(ShaderType type) -> std::string;
	auto CompileShader(ID3DBlob** pBlob, ID3DBlob** pErrorBlob, const std::filesystem::path& shaderFilePath, LPCSTR const target) -> HRESULT;

	class ShaderBase
	{
	public:
		virtual ~ShaderBase() = default;
	};

	class VertexShader : public ShaderBase
	{
	public:
		VertexShader(ID3D11Device* pDevice, std::filesystem::path path);
		VertexShader(ID3D11Device* pDevice, ShaderBlob blob);
		virtual ~VertexShader();
		void Bind(ID3D11DeviceContext* pContext) const;
		[[nodiscard]] auto GetBufferPointer() const -> LPVOID { return m_ShaderBlob.Blob->GetBufferPointer(); }
		[[nodiscard]] auto GetBufferSize() const -> SIZE_T { return m_ShaderBlob.Blob->GetBufferSize(); }
		[[nodiscard]] auto GetD3D11VertexShader() const -> const ComPtr<ID3D11VertexShader>& { return m_pShader; }
	private:
		ID3D11Device* m_pDevice = nullptr;
		ShaderBlob m_ShaderBlob{};
		ComPtr<ID3D11VertexShader> m_pShader = nullptr;
		std::filesystem::path m_Path;

		friend class MeshRendererComponent;
	};

	class PixelShader : public ShaderBase
	{
	public:
		PixelShader(ID3D11Device* pDevice, std::filesystem::path path);
		PixelShader(ID3D11Device* pDevice, ShaderBlob blob);
		virtual ~PixelShader();
		void Bind(ID3D11DeviceContext* pContext) const;
		[[nodiscard]] auto GetBufferPointer() const -> LPVOID { return m_ShaderBlob.Blob->GetBufferPointer(); }
		[[nodiscard]] auto GetBufferSize() const -> SIZE_T { return m_ShaderBlob.Blob->GetBufferSize(); }
		[[nodiscard]] auto GetD3D11PixelShader() const -> const ComPtr<ID3D11PixelShader>& { return m_pShader; }
		[[nodiscard]] auto GetPath() const -> const std::filesystem::path& { return m_Path; }
	private:
		ID3D11Device* m_pDevice = nullptr;
		ShaderBlob m_ShaderBlob{};
		ComPtr<ID3D11PixelShader> m_pShader = nullptr;
		std::filesystem::path m_Path;

		friend class MeshRendererComponent;
	};

	using VertexShaderRef = std::shared_ptr<VertexShader>;
	using PixelShaderRef = std::shared_ptr<PixelShader>;
}
