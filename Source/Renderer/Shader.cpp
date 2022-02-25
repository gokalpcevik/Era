#include "Shader.h"

namespace Era
{
	auto CompileShader(ID3DBlob** pBlob, ID3DBlob** pErrorBlob, LPCWSTR const file, LPCSTR const target) -> HRESULT
	{
		if(!std::filesystem::exists(file))
		{
			size_t outputSize = wcslen(file) + 1; // +1 for null terminator
			char* fileStr = new char[outputSize];
			size_t charsConverted = 0;
			const wchar_t* inputW = file;
			wcstombs_s(&charsConverted, fileStr, outputSize, inputW, 256);
			ERA_ERROR("Cannot find the specified shader file: {0}", fileStr);
			return E_FAIL;
		}
		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif
		LPCSTR const entryPoint = "main";
		HRESULT result = D3DCompileFromFile(file, nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint,
			target,
			0u,
			0u,
			pBlob,
			pErrorBlob);
		return result;
	}

	VertexShader::VertexShader(ID3D11Device* pDevice, std::filesystem::path path) : m_pDevice(pDevice), m_Path(std::move(path))
	{
		DX_RESULT(CompileShader(&m_pBlob, &m_pErrorBlob, m_Path.c_str(), "vs_5_0"));
		if(m_pBlob)
			DX_RESULT(pDevice->CreateVertexShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pShader));
		else if(m_pErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_pErrorBlob->GetBufferPointer()));
		}
	}

	VertexShader::~VertexShader()
	{
		if (m_pBlob)
			m_pBlob->Release();
		if (m_pErrorBlob)
			m_pErrorBlob->Release();
	}

	void VertexShader::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->VSSetShader(m_pShader.Get(), nullptr, 0u);
	}

	PixelShader::PixelShader(ID3D11Device* pDevice, std::filesystem::path path) : m_pDevice(pDevice), m_Path(std::move(path))
	{
		DX_RESULT(CompileShader(&m_pBlob, &m_pErrorBlob, m_Path.c_str(), "ps_5_0"));
		if (m_pBlob)
			DX_RESULT(pDevice->CreatePixelShader(m_pBlob->GetBufferPointer(), m_pBlob->GetBufferSize(), nullptr, &m_pShader));
		else if (m_pErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_pErrorBlob->GetBufferPointer()));
		}
	}

	PixelShader::~PixelShader()
	{
		if (m_pBlob)
			m_pBlob->Release();
		if (m_pErrorBlob)
			m_pErrorBlob->Release();
	}

	void PixelShader::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->PSSetShader(m_pShader.Get(), nullptr, 0u);
	}
}
