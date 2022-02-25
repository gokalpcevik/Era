#pragma once

#include <d3d11_3.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <string_view>
#include <filesystem>
#include "ErrorChecker.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;

	static auto CompileShader(ID3DBlob** pBlob, ID3DBlob** pErrorBlob, LPCWSTR const file, LPCSTR const target) ->
	HRESULT;

	class VertexShader
	{
	public:
		VertexShader(ID3D11Device* pDevice, std::filesystem::path path);
		~VertexShader();
		void Bind(ID3D11DeviceContext* pContext) const;

		[[nodiscard]] auto GetBlob() const -> ID3DBlob* { return m_pBlob; }
		[[nodiscard]] auto GetErrorBlob() const -> ID3DBlob* { return m_pErrorBlob; }
		[[nodiscard]] auto GetD3D11VertexShader() const -> const ComPtr<ID3D11VertexShader>& { return m_pShader; }
		[[nodiscard]] auto GetPath() const -> const std::filesystem::path& { return m_Path; }
	private:
		ID3D11Device* m_pDevice = nullptr;
		ID3DBlob* m_pBlob = nullptr;
		ID3DBlob* m_pErrorBlob = nullptr;
		ComPtr<ID3D11VertexShader> m_pShader = nullptr;
		std::filesystem::path m_Path;
	};

	class PixelShader
	{
	public:
		PixelShader(ID3D11Device* pDevice, std::filesystem::path path);
		~PixelShader();
		void Bind(ID3D11DeviceContext* pContext) const;
		[[nodiscard]] auto GetBlob() const -> ID3DBlob* { return m_pBlob; }
		[[nodiscard]] auto GetErrorBlob() const -> ID3DBlob* { return m_pErrorBlob; }
		[[nodiscard]] auto GetD3D11PixelShader() const -> const ComPtr<ID3D11PixelShader>& { return m_pShader; }
		[[nodiscard]] auto GetPath() const -> const std::filesystem::path& { return m_Path; }
	private:
		ID3D11Device* m_pDevice = nullptr;
		ID3DBlob* m_pBlob = nullptr;
		ID3DBlob* m_pErrorBlob = nullptr;
		ComPtr<ID3D11PixelShader> m_pShader = nullptr;
		std::filesystem::path m_Path;
	};

	using VertexShaderRef = std::shared_ptr<VertexShader>;
	using PixelShaderRef = std::shared_ptr<PixelShader>;
}
