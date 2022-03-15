#pragma once
#include <DirectXMath.h>
#include <filesystem>
#include <future>
#include <mutex>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "../Core/Log.h"
#include "Asset.h"

namespace Era
{
	namespace DX = DirectX;

	class MeshAsset : public Asset
	{
	public:
		MeshAsset(std::filesystem::path path, uint32_t meshIndex);
		MeshAsset();
		virtual ~MeshAsset() = default;
	public:
		[[nodiscard]] auto IsImportSuccess() const -> bool								  { return m_Success; }
		[[nodiscard]] auto HasTextureCoords(size_t index) const -> bool					  { return m_HasTextureCoords[index]; }
		[[nodiscard]] auto GetNumVertices() const -> size_t								  { return m_NumVertices; }
		[[nodiscard]] auto GetNumFaces() const -> size_t								  { return m_NumFaces; }
		[[nodiscard]] auto GetNumIndices() const -> size_t								  { return m_Indices.size(); }
		[[nodiscard]] auto GetVertexPositions() const -> const std::vector<DX::XMFLOAT3>& { return m_VertexPositions; }
		[[nodiscard]] auto GetVertexNormals() const -> const std::vector<DX::XMFLOAT3>&   { return m_VertexNormals; }
		[[nodiscard]] auto GetTextureCoordinates(size_t index) const -> std::vector<DX::XMFLOAT2>;
		[[nodiscard]] auto GetIndices() const -> std::vector<uint32_t>					  { return m_Indices; }

		void Import();

	private:
		std::vector<DX::XMFLOAT3> m_VertexPositions{};
		std::vector<DX::XMFLOAT3> m_VertexNormals{};
		std::vector<uint32_t> m_Indices{};
		std::array<std::vector<DX::XMFLOAT2>,8> m_TexCoords{};
		std::array<bool, 8> m_HasTextureCoords{};
		size_t m_NumVertices{ 0 };
		size_t m_NumFaces{ 0 };
		uint32_t m_MeshIndex = 0;
		bool m_Success{false};
	};
}
