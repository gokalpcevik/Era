#include "MeshRendererComponent.h"

namespace Era
{
	MeshRendererComponent::MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset)
	{
		CreateVertexShader(pDevice);
		CreatePixelShader(pDevice);
		CreateInputLayout(pDevice, m_VertexShader);
		CreateIndexBuffer(pDevice, meshAsset);
		CreateVertexBuffer(pDevice, meshAsset);
		CreateVertexShaderConstantBuffer(pDevice);
		CreatePixelShaderConstantBuffer(pDevice);

		m_Texture2DRef = std::make_shared<Texture2D>(pDevice, "Assets/default_cube.png");
		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		m_SamplerState = std::make_shared<SamplerState>(pDevice,samplerDesc, SamplerStateShaderType::Pixel);

	}

	MeshRendererComponent::MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset,
		const std::filesystem::path& texturePath)
	{
		CreateVertexShader(pDevice);
		CreatePixelShader(pDevice);
		CreateInputLayout(pDevice, m_VertexShader);
		CreateIndexBuffer(pDevice, meshAsset);
		CreateVertexBuffer(pDevice, meshAsset);
		CreateVertexShaderConstantBuffer(pDevice);
		CreatePixelShaderConstantBuffer(pDevice);

		m_Texture2DRef = std::make_shared<Texture2D>(pDevice, texturePath);
		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		m_SamplerState = std::make_shared<SamplerState>(pDevice, samplerDesc, SamplerStateShaderType::Pixel);
	}

	MeshRendererComponent::MeshRendererComponent(ID3D11Device* pDevice, const MeshAsset& meshAsset,
		const MaterialRef& material) : m_Material(material)
	{
		CreateVertexBuffer(pDevice, meshAsset);
		CreateIndexBuffer(pDevice, meshAsset);
	}

	void MeshRendererComponent::SetWorldViewProjection(ID3D11DeviceContext* pContext,
	                                                   const DX::XMMATRIX& WorldViewProjection)
	{
		m_VSConstantBufferData.WorldViewProjection = WorldViewProjection;
		m_VSConstantBuffer->Update(pContext, m_VSConstantBufferData);
	}

	void MeshRendererComponent::SetWorld(ID3D11DeviceContext* pContext, const DX::XMMATRIX& World)
	{
		m_VSConstantBufferData.World = World;
		m_VSConstantBuffer->Update(pContext, m_VSConstantBufferData);
	}

	void MeshRendererComponent::UpdateLightData(ID3D11DeviceContext* pContext, const PSConstantBufferData& data) const
	{
		m_PSConstantBuffer->Update(pContext, data);
	}

	void MeshRendererComponent::CreateVertexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset)
	{
		std::vector<Vertex> vertices{};
		vertices.reserve(meshAsset.GetNumVertices());
		for (size_t i = 0; i < meshAsset.GetNumVertices(); ++i)
		{
			DX::XMFLOAT3 Pos{ meshAsset.GetVertexPositions()[i].x ,meshAsset.GetVertexPositions()[i].y ,meshAsset.GetVertexPositions()[i].z };
			DX::XMFLOAT3 Normal{ meshAsset.GetVertexNormals()[i].x,meshAsset.GetVertexNormals()[i].y,meshAsset.GetVertexNormals()[i].z };
			vertices.emplace_back(Pos, Normal, meshAsset.GetTextureCoordinates(0u)[i],0);
		}
		m_VertexBuffer = std::make_shared<VertexBuffer<Vertex>>(pDevice, vertices.data(), vertices.size());
	}

	void MeshRendererComponent::CreateIndexBuffer(ID3D11Device* pDevice, const MeshAsset& meshAsset)
	{
		m_IndexBuffer = std::make_shared<IndexBuffer>(pDevice, meshAsset.GetIndices().data(), meshAsset.GetNumIndices());
	}

	void MeshRendererComponent::CreateVertexShader(ID3D11Device* pDevice)
	{
		const std::filesystem::path VSPath = "Shaders/Lit.vshader";
		ShaderBlob VSBlob{ nullptr,nullptr,ShaderType::Vertex };
		if (!ShaderLibrary::Exists(VSPath))
		{
			DX_RESULT(CompileShader(&VSBlob.Blob, &VSBlob.ErrorBlob, VSPath, "vs_5_0"));
			if (VSBlob.ErrorBlob)
			{
				ERA_ERROR("Error while compiling shader {0} \n {1}", VSPath.string(),
					static_cast<const char*>(VSBlob.ErrorBlob->GetBufferPointer()));
			}
			else if (VSBlob.Blob)
			{
				ShaderLibrary::AddShader(VSPath, VSBlob);
				VertexLayout layout(VSBlob.Blob);
			}
		}
		else
		{
			VSBlob = ShaderLibrary::GetShaderBlob(VSPath);
		}
		m_VertexShader = std::make_shared<VertexShader>(pDevice, VSBlob);
	}

	void MeshRendererComponent::CreatePixelShader(ID3D11Device* pDevice)
	{
		const std::filesystem::path PSPath = "Shaders/Lit.pshader";
		ShaderBlob PSBlob{ nullptr,nullptr,ShaderType::Pixel };
		if (!ShaderLibrary::Exists(PSPath))
		{
			DX_RESULT(CompileShader(&PSBlob.Blob, &PSBlob.ErrorBlob, PSPath, "ps_5_0"));
			if (PSBlob.ErrorBlob)
			{
				ERA_ERROR("Error while compiling shader {0} \n {1}", PSPath.string(),
					static_cast<const char*>(PSBlob.ErrorBlob->GetBufferPointer()));
				ShaderLibrary::AddShader(PSPath, PSBlob);
			}
		}
		else
		{
			PSBlob = ShaderLibrary::GetShaderBlob(PSPath);
		}

		m_PixelShader = std::make_shared<PixelShader>(pDevice, PSBlob);
	}

	void MeshRendererComponent::CreateInputLayout(ID3D11Device* pDevice, const VertexShaderRef& vs)
	{
		/*constexpr D3D11_INPUT_ELEMENT_DESC elems[] =
		{
			{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
			{"UV",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}			  ,
			{"USE_TEXTURES",0,DXGI_FORMAT_R8_UINT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
		};

		if (m_VertexShader->GetBufferPointer())
			m_InputLayout = std::make_shared<InputLayout>(pDevice, elems, _countof(elems),
				m_VertexShader->GetBufferPointer()
				, m_VertexShader->GetBufferSize());*/

		if(m_VertexShader)
		{
			m_InputLayout = std::make_shared<InputLayout>(pDevice, vs->m_ShaderBlob.Blob,VertexLayout(vs->m_ShaderBlob.Blob));
		}
	}

	void MeshRendererComponent::CreateVertexShaderConstantBuffer(ID3D11Device* pDevice)
	{
		m_VSConstantBuffer = std::make_shared<ConstantBuffer<VSConstantBufferData>>(pDevice, m_VSConstantBufferData, ConstantBufferType::Vertex);
	}

	void MeshRendererComponent::CreatePixelShaderConstantBuffer(ID3D11Device* pDevice)
	{
		m_PSConstantBuffer = std::make_shared<ConstantBuffer<PSConstantBufferData>>(pDevice, m_PSConstantBufferData, ConstantBufferType::Pixel);
	}
}
