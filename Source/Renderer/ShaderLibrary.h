#pragma once
#include <unordered_map>
#include "Shader.h"

namespace Era
{
	class ShaderLibrary
	{
	public:
		ShaderLibrary() = default;

		static void AddShader(const std::filesystem::path& shaderFilePath, ShaderBlob blob);
		static auto RemoveShader(const std::filesystem::path& shaderFilePath) -> bool;
		static auto GetShaderBlob(const std::filesystem::path& shaderFilePath) -> ShaderBlob;
		static auto GetOrAddShaderBlob(const std::filesystem::path& shaderFilePath) -> ShaderBlob;
		static auto Exists(const std::filesystem::path& shaderFilePath) -> bool;

	private:
		static std::unordered_map<std::filesystem::path, ShaderBlob,ShaderBlobHashFn> m_ShaderNameToShaderBlob;
	};
}
