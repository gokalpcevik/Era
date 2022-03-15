#include "Material.h"

namespace Era
{
	Material::Material(std::filesystem::path vertexShaderPath, std::filesystem::path pixelShaderPath)
	: m_VertexShaderPath(std::move(vertexShaderPath)) , m_PixelShaderPath(std::move(pixelShaderPath))
	{
		ShaderBlob VSBlob{ nullptr,nullptr,ShaderType::Vertex };
		if (!ShaderLibrary::Exists(m_VertexShaderPath))
		{
			DX_RESULT(CompileShader(&VSBlob.Blob, &VSBlob.ErrorBlob, m_VertexShaderPath, "vs_5_0"));
			if (VSBlob.ErrorBlob)
			{
				ERA_ERROR("Error while compiling shader {0} \n {1}", m_VertexShaderPath.string(),
					static_cast<const char*>(VSBlob.ErrorBlob->GetBufferPointer()));
			}
			else if (VSBlob.Blob)
			{
				ShaderLibrary::AddShader(m_VertexShaderPath, VSBlob);
			}
		}
		else
		{
			VSBlob = ShaderLibrary::GetShaderBlob(m_VertexShaderPath);
		}

		ShaderBlob PSBlob{ nullptr,nullptr,ShaderType::Pixel };
		if (!ShaderLibrary::Exists(m_PixelShaderPath))
		{
			DX_RESULT(CompileShader(&PSBlob.Blob, &PSBlob.ErrorBlob, m_PixelShaderPath, "ps_5_0"));
			if (PSBlob.ErrorBlob)
			{
				ERA_ERROR("Error while compiling shader {0} \n {1}", m_PixelShaderPath.string(),
					static_cast<const char*>(PSBlob.ErrorBlob->GetBufferPointer()));
				ShaderLibrary::AddShader(m_PixelShaderPath, PSBlob);
			}
		}
		else
		{
			PSBlob = ShaderLibrary::GetShaderBlob(m_PixelShaderPath);
		}
	}

	void Material::SetDiffuse(const Texture2DRef& diffuse)
	{
		m_Diffuse = diffuse;
	}

	void Material::SetSpecular(const Texture2DRef& specular)
	{
		m_Specular = specular;
	}

	
}
