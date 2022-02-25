#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "../Log.h"
#include "Asset.h"

namespace Era
{
	namespace DX = DirectX;

	class MeshAsset : public Asset
	{
	public:
		MeshAsset(std::filesystem::path path,uint32_t meshIndex);
		virtual ~MeshAsset() = default;
	public:
		[[nodiscard]] auto IsImportSuccess() const -> bool								  { return m_Success; }
		[[nodiscard]] auto HasTextureCoords(size_t index) const -> bool					  { return m_HasTextureCoords[index]; }
		[[nodiscard]] auto GetNumVertices() const -> size_t								  { return m_NumVertices; }
		[[nodiscard]] auto GetNumFaces() const -> size_t								  { return m_NumFaces; }
		[[nodiscard]] auto GetNumIndices() const -> size_t								  { return m_Indices.size(); }
		[[nodiscard]] auto GetVertexPositions() const -> const std::vector<DX::XMFLOAT3>& { return m_VertexPositions; }
		[[nodiscard]] auto GetVertexNormals() const -> const std::vector<DX::XMFLOAT3>&   { return m_VertexNormals; }
		[[nodiscard]] auto GetIndices() const -> std::vector<uint32_t>					  { return m_Indices; }
	private:
		void Import(const std::filesystem::path& path);

	private:
		std::vector<DX::XMFLOAT3> m_VertexPositions{};
		std::vector<DX::XMFLOAT3> m_VertexNormals{};
		std::vector<uint32_t> m_Indices{};
		size_t m_NumVertices{ 0 };
		size_t m_NumFaces{ 0 };
		uint32_t m_MeshIndex = 0;
		std::array<bool, 8> m_HasTextureCoords{};
		bool m_Success{false};
	};
}
