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

	static HRESULT CompileShader(ID3DBlob* pBlob, ID3DBlob* pErrorBlob, LPCWSTR const file,LPCSTR const target);

	class VertexShader
	{
	public:
		VertexShader(ID3D11Device* pDevice, std::filesystem::path path);
		~VertexShader();
		void Bind(ID3D11DeviceContext* pContext) const;

		[[nodiscard]] ID3DBlob* GetBlob() const { return m_pBlob; }
		[[nodiscard]] ID3DBlob* GetErrorBlob() const { return m_pErrorBlob; }
		[[nodiscard]] const ComPtr<ID3D11VertexShader>& GetD3D11VertexShader() const { return m_pShader; }
		[[nodiscard]] const std::filesystem::path& GetPath() const { return m_Path; }
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
		[[nodiscard]] ID3DBlob* GetBlob() const { return m_pBlob; }
		[[nodiscard]] ID3DBlob* GetErrorBlob() const { return m_pErrorBlob; }
		[[nodiscard]] const ComPtr<ID3D11PixelShader>& GetD3D11PixelShader() const { return m_pShader; }
		[[nodiscard]] const std::filesystem::path& GetPath() const { return m_Path; }
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
