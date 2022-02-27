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
		static bool RemoveShader(const std::filesystem::path& shaderFilePath);
		static ShaderBlob GetShaderBlob(const std::filesystem::path& shaderFilePath);
		static bool Exists(const std::filesystem::path& shaderFilePath);

	private:
		static std::unordered_map<std::filesystem::path, ShaderBlob,ShaderBlobHashFn> m_ShaderNameToShaderBlob;
	};
}
