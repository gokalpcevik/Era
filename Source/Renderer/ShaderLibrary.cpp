#include "ShaderLibrary.h"

namespace Era
{
	std::unordered_map<std::filesystem::path, ShaderBlob,ShaderBlobHashFn> ShaderLibrary::m_ShaderNameToShaderBlob{};

	void ShaderLibrary::AddShader(const std::filesystem::path& shaderFilePath, ShaderBlob blob)
	{
		m_ShaderNameToShaderBlob[shaderFilePath.filename().string()] = blob;
	}

	auto ShaderLibrary::RemoveShader(const std::filesystem::path& shaderFilePath) -> bool
	{
		if (m_ShaderNameToShaderBlob.find(shaderFilePath.filename().string()) != m_ShaderNameToShaderBlob.end())
		{
			m_ShaderNameToShaderBlob[shaderFilePath].Blob->Release();
			if (m_ShaderNameToShaderBlob[shaderFilePath].ErrorBlob)
			{
				m_ShaderNameToShaderBlob[shaderFilePath].ErrorBlob->Release();
			}
			m_ShaderNameToShaderBlob.erase(shaderFilePath.filename().string());
			return true;
		}
		return false;
	}

	auto ShaderLibrary::GetShaderBlob(const std::filesystem::path& shaderFilePath) -> ShaderBlob
	{
		return m_ShaderNameToShaderBlob[shaderFilePath.filename().string()];
	}

	auto ShaderLibrary::GetOrAddShaderBlob(const std::filesystem::path& shaderFilePath) -> ShaderBlob
	{
		auto extension = GetShaderTypeFromExtension(shaderFilePath.extension());
		ShaderBlob blob{ nullptr,nullptr,extension };
		auto const shaderType = extension;
		if (shaderType == ShaderType::Unknown)
		{
			ERA_ERROR("Unknown shader extension file!: {0}", extension);
			return { nullptr,nullptr };
		}

		if (!ShaderLibrary::Exists(shaderFilePath))
		{
			DX_RESULT(CompileShader(&blob.Blob, &blob.ErrorBlob, shaderFilePath, Era::GetShaderTargetFromType(shaderType).c_str()));
			ShaderLibrary::AddShader(shaderFilePath, blob);
			return blob;
		}
		else
		{
			return m_ShaderNameToShaderBlob[shaderFilePath.filename().string()];
		}
	}

	auto ShaderLibrary::Exists(const std::filesystem::path& shaderFilePath) -> bool
	{
		return m_ShaderNameToShaderBlob.find(shaderFilePath.filename().string()) != m_ShaderNameToShaderBlob.end();
	}
}
