#include "MeshAsset.h"

namespace Era
{
	
	MeshAsset::MeshAsset(std::filesystem::path path, uint32_t meshIndex) : Asset(path) , m_MeshIndex(meshIndex)
	{
	}

	MeshAsset::MeshAsset() : Asset("")
	{
	}

	auto MeshAsset::GetTextureCoordinates(size_t index) const -> std::vector<DX::XMFLOAT2>
	{
		return m_TexCoords[index];
	}


	void MeshAsset::Import()
	{
		Assimp::Importer importer;
		auto const* pScene = importer.ReadFile(m_Path.string().c_str(), aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded
		);
		if(!pScene)
		{
			ERA_ERROR("Error while importing mesh: {0}", importer.GetErrorString());
			m_Success = false;
			return;
		}
		
		auto const* pMesh = pScene->mMeshes[m_MeshIndex];
		m_NumVertices = pMesh->mNumVertices;

		m_VertexPositions.reserve(pMesh->mNumVertices);

		/*
		if(parseTexCoords)
		{
			for (auto i = 0; i < 8; ++i)
			{
				m_HasTextureCoords[i] = pMesh->HasTextureCoords(i);
				if (pMesh->HasTextureCoords(i))
				{
					for (uint32_t j = 0; j < pMesh->mNumVertices; ++j)
					{
						m_TexCoords[i].push_back({ pMesh->mTextureCoords[i][j].x,pMesh->mTextureCoords[i][j].y,pMesh->mTextureCoords[i][j].z });
					}
				}
			}
		}*/
		if (pMesh->HasTextureCoords(0))
		{
			for (uint32_t j = 0; j < pMesh->mNumVertices; ++j)
			{
				m_TexCoords[0].push_back({ pMesh->mTextureCoords[0][j].x,pMesh->mTextureCoords[0][j].y});
			}
		}


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
