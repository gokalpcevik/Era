#include "Shader.h"

namespace Era
{
	auto GetShaderTypeFromExtension(const std::filesystem::path& extension) -> ShaderType  // NOLINT(clang-diagnostic-unused-function)
	{
		if (extension == ".vshader")
			return ShaderType::Vertex;
		else if (extension == ".pshader")
			return ShaderType::Pixel;
		else if (extension == ".cshader")
			return ShaderType::Compute;
		else if (extension == ".gshader")
			return ShaderType::Geometry;
		return {};
	}

	auto CompileShader(ID3DBlob** pBlob, ID3DBlob** pErrorBlob, const std::filesystem::path& shaderFilePath, LPCSTR const target) -> HRESULT
	{
		if(!std::filesystem::exists(shaderFilePath))
		{
			ERA_ERROR("Cannot find the specified shader file: {0}", shaderFilePath.string());
			return E_FAIL;
		}
		if(!shaderFilePath.has_extension())
		{
			ERA_ERROR("Shader files need to have a \".*shader\" extension!, {0}",shaderFilePath.string());
			return E_FAIL;
		}

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif
		LPCSTR const entryPoint = "main";
		HRESULT result = D3DCompileFromFile(shaderFilePath.c_str(), nullptr,
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
		DX_RESULT(CompileShader(&m_ShaderBlob.Blob, &m_ShaderBlob.ErrorBlob, m_Path.c_str(), "vs_5_0"));
		if(m_ShaderBlob.Blob)
			DX_RESULT(pDevice->CreateVertexShader(m_ShaderBlob.Blob->GetBufferPointer(), m_ShaderBlob.Blob->GetBufferSize(), nullptr, &m_pShader));
		else if(m_ShaderBlob.ErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_ShaderBlob.ErrorBlob->GetBufferPointer()));
		}
	}

	VertexShader::VertexShader(ID3D11Device* pDevice, ShaderBlob blob) : m_pDevice(pDevice),m_ShaderBlob(blob)
	{
		if (m_ShaderBlob.Blob)
			DX_RESULT(pDevice->CreateVertexShader(m_ShaderBlob.Blob->GetBufferPointer(), m_ShaderBlob.Blob->GetBufferSize(), nullptr, &m_pShader));
		else if (m_ShaderBlob.ErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_ShaderBlob.ErrorBlob->GetBufferPointer()));
		}
	}

	VertexShader::~VertexShader()
	{
		if (m_ShaderBlob.Blob)
			m_ShaderBlob.Blob->Release();
		if (m_ShaderBlob.ErrorBlob)
			m_ShaderBlob.ErrorBlob->Release();
	}

	void VertexShader::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->VSSetShader(m_pShader.Get(), nullptr, 0u);
	}

	PixelShader::PixelShader(ID3D11Device* pDevice, std::filesystem::path path) : m_pDevice(pDevice), m_Path(std::move(path))
	{
		DX_RESULT(CompileShader(&m_ShaderBlob.Blob, &m_ShaderBlob.ErrorBlob, m_Path.c_str(), "ps_5_0"));
		if (m_ShaderBlob.Blob)
			DX_RESULT(pDevice->CreatePixelShader(m_ShaderBlob.Blob->GetBufferPointer(), m_ShaderBlob.Blob->GetBufferSize(), nullptr, &m_pShader));
		else if (m_ShaderBlob.ErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_ShaderBlob.ErrorBlob->GetBufferPointer()));
		}
	}

	PixelShader::PixelShader(ID3D11Device* pDevice, ShaderBlob blob) : m_pDevice(pDevice), m_ShaderBlob(blob)
	{
		if (m_ShaderBlob.Blob)
			DX_RESULT(pDevice->CreatePixelShader(m_ShaderBlob.Blob->GetBufferPointer(), m_ShaderBlob.Blob->GetBufferSize(), nullptr, &m_pShader));
		else if (m_ShaderBlob.ErrorBlob)
		{
			ERA_ERROR("Error while compiling shader {0} \n {1}", m_Path.string(), reinterpret_cast<const char*>(m_ShaderBlob.ErrorBlob->GetBufferPointer()));
		}
	}

	PixelShader::~PixelShader()
	{
		if (m_ShaderBlob.Blob)
			m_ShaderBlob.Blob->Release();
		if (m_ShaderBlob.ErrorBlob)
			m_ShaderBlob.ErrorBlob->Release();
	}

	void PixelShader::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->PSSetShader(m_pShader.Get(), nullptr, 0u);
	}
}
