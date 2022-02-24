#include "MeshAsset.h"

namespace Era
{
	
	MeshAsset::MeshAsset(std::filesystem::path path) : m_Path(std::move(path))
	{
		this->Import(m_Path);
	}

	void MeshAsset::Import(const std::filesystem::path& path)
	{
		Assimp::Importer importer;
		auto const* pScene = importer.ReadFile(m_Path.string().c_str(), aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices | aiProcess_ConvertToLeftHanded
		);
		auto const* pMesh = pScene->mMeshes[0];
		m_VertexPositions.reserve(pMesh->mNumVertices);
		m_NumVertices = pMesh->mNumVertices;

		for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
		{
			m_VertexPositions.emplace_back(pMesh->mVertices[i].x, pMesh->mVertices[i].y, pMesh->mVertices[i].z);
			m_VertexNormals.emplace_back(pMesh->mNormals[i].x, pMesh->mNormals[i].y, pMesh->mNormals[i].z);
		}

		m_Indices.reserve(pMesh->mNumFaces * 3);
		m_NumFaces = pMesh->mNumFaces;

		for (uint32_t i = 0; i < pMesh->mNumFaces; ++i)
		{
			const auto& face = pMesh->mFaces[i];
			m_Indices.push_back(face.mIndices[0]);
			m_Indices.push_back(face.mIndices[1]);
			m_Indices.push_back(face.mIndices[2]);
		}
	}
}
