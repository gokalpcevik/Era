#include "ShaderLibrary.h"

namespace Era
{
	std::unordered_map<std::filesystem::path, ShaderBlob,ShaderBlobHashFn> ShaderLibrary::m_ShaderNameToShaderBlob{};

	void ShaderLibrary::AddShader(const std::filesystem::path& shaderFilePath, ShaderBlob blob)
	{
		if (m_ShaderNameToShaderBlob.find(shaderFilePath.filename().string()) == m_ShaderNameToShaderBlob.end())
			m_ShaderNameToShaderBlob[shaderFilePath.filename().string()] = blob;
	}

	auto ShaderLibrary::RemoveShader(const std::filesystem::path& shaderFilePath) -> bool
	{
		if (m_ShaderNameToShaderBlob.find(shaderFilePath.filename().string()) != m_ShaderNameToShaderBlob.end())
		{
			m_ShaderNameToShaderBlob.erase(shaderFilePath.filename().string());
			return true;
		}
		return false;
	}

	auto ShaderLibrary::GetShaderBlob(const std::filesystem::path& shaderFilePath) -> ShaderBlob
	{
		return m_ShaderNameToShaderBlob[shaderFilePath.filename().string()];
	}

	auto ShaderLibrary::Exists(const std::filesystem::path& shaderFilePath) -> bool
	{
		return m_ShaderNameToShaderBlob.find(shaderFilePath.filename().string()) != m_ShaderNameToShaderBlob.end();
	}
}
