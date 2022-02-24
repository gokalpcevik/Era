#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Era
{
	namespace DX = DirectX;


	class MeshAsset
	{
	public:
		MeshAsset(std::filesystem::path path);

		[[nodiscard]] size_t GetNumVertices() const { return m_NumVertices; }
		[[nodiscard]] size_t GetNumFaces() const { return m_NumFaces; }
		[[nodiscard]] size_t GetNumIndices() const { return m_Indices.size(); }
		[[nodiscard]] const std::vector<DX::XMFLOAT3>& GetVertexPositions() const { return m_VertexPositions; }
		[[nodiscard]] const std::vector<DX::XMFLOAT3>& GetVertexNormals() const { return m_VertexNormals; }
		[[nodiscard]] std::vector<uint32_t> GetIndices() const { return m_Indices; }
		[[nodiscard]] const std::filesystem::path& GetPath() const { return m_Path; }
	private:
		void Import(const std::filesystem::path& path);

	private:
		std::filesystem::path m_Path{};
		std::vector<DX::XMFLOAT3> m_VertexPositions{};
		std::vector<DX::XMFLOAT3> m_VertexNormals{};
		std::vector<uint32_t> m_Indices{};
		
		size_t m_NumVertices{ 0 };
		size_t m_NumFaces{ 0 };
		
	};
}