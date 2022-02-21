#include "Mesh.h"

namespace Era
{
	/*
	Mesh::Mesh(const GraphicsDevice& device, std::string_view path)
	{
		Assimp::Importer importer{};

		const aiScene* scene = importer.ReadFile(path.data(),aiProcess_Triangulate|
		aiProcess_JoinIdenticalVertices
		|aiProcess_ConvertToLeftHanded);

		if(!scene)
		{
			ERA_ERROR("Error while importing {0} \n Description: {1}", path,importer.GetErrorString());
			return;
		}

		if(aiMesh* pMesh = scene->mMeshes[0])
		{
			m_Vertices.reserve(pMesh->mNumVertices);
			for(size_t i = 0; i < pMesh->mNumVertices; ++i)
			{
				m_Vertices.push_back({ *(DX::XMFLOAT3*)(&pMesh->mVertices[i]) });
			}

			m_Indices.reserve(pMesh->mNumFaces * 3);
			for(size_t i = 0; i < pMesh->mNumFaces; ++i)
			{
				const auto& face = pMesh->mFaces[i];
				m_Indices.push_back(face.mIndices[0]);
				m_Indices.push_back(face.mIndices[1]);
				m_Indices.push_back(face.mIndices[2]);
			}
			m_VertexBuffer = device.CreateVertexBuffer<MeshVertex>(m_Vertices.data(), m_Vertices.size());
			m_IndexBuffer = device.CreateIndexBuffer(m_Indices.data(), m_Indices.size());
			m_VertexShader = device.CreateVertexShader(L"shaders\\mesh.vsh");
			m_PixelShader = device.CreatePixelShader(L"shaders\\mesh.psh");


			D3D11_INPUT_ELEMENT_DESC elems[] =
			{
				{"Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0}
			};
			m_InputLayout = device.CreateInputLayout(elems, _countof(elems),
				m_VertexShader->GetBlob()->GetBufferPointer(),
				m_VertexShader->GetBlob()->GetBufferSize());

		}
	}

	void Mesh::Draw(const DeviceContext& context) const
	{
		m_VertexBuffer->Bind(context.GetD3D11DeviceContext().Get());
		m_InputLayout->Bind(context.GetD3D11DeviceContext().Get());
		m_VertexShader->Bind(context.GetD3D11DeviceContext().Get());
		m_PixelShader->Bind(context.GetD3D11DeviceContext().Get());
	}
	*/
}
