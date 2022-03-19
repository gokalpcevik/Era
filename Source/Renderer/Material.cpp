#include "Material.h"

namespace Era
{
	Material::Material(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, std::filesystem::path vertexShaderPath,
		std::filesystem::path pixelShaderPath)
	: m_Device(pDevice), m_DeviceCtx(pContext), m_VertexShaderPath(std::move(vertexShaderPath)),
	  m_PixelShaderPath(std::move(pixelShaderPath))
	{
		ShaderBlob VSBlob{ nullptr,nullptr,ShaderType::Vertex };
		ShaderBlob PSBlob{ nullptr,nullptr, ShaderType::Pixel };
		VSBlob = ShaderLibrary::GetOrAddShaderBlob(m_VertexShaderPath);
		PSBlob = ShaderLibrary::GetOrAddShaderBlob(m_PixelShaderPath);
		m_VertexShader = std::make_shared<VertexShader>(m_Device, VSBlob);
		m_PixelShader = std::make_shared<PixelShader>(m_Device, PSBlob);
		m_DefaultConstantBuffer = std::make_shared<ConstantBuffer<PSDefaultCBufferData>>(m_Device,m_DefaultCBData,ConstantBufferType::Pixel);
		VertexLayout layout(VSBlob.Blob);
		m_InputLayout = std::make_shared<InputLayout>(m_Device, VSBlob.Blob, std::move(layout));
		m_RasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(FillMode::FillSolid);
		m_RasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(CullMode::CullBack);
		m_RasterizerState = std::make_shared<RasterizerState>(m_Device,m_RasterizerDesc);
		m_Textures = std::make_shared<TextureArray>();

		D3D11_SAMPLER_DESC samplerDesc{};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		m_SamplerState = std::make_shared<SamplerState>(m_Device, samplerDesc, SamplerStateShaderType::Pixel);

		for(size_t i = 1; i < GetNumberOfConstantBuffers(PSBlob.Blob);++i)
		{
			m_ConstantBuffers.emplace_back(std::make_shared<DynamicConstantBuffer>(m_Device, PSBlob.Blob, i, nullptr));
		}
	}


	void Material::SetCullMode(CullMode mode)
	{
		m_CullMode = mode;
		m_RasterizerDesc.CullMode = static_cast<D3D11_CULL_MODE>(mode);
		m_RasterizerState->Reset(m_Device,m_RasterizerDesc);
	}

	void Material::SetFillMode(FillMode mode)
	{
		m_FillMode = mode;
		m_RasterizerDesc.FillMode = static_cast<D3D11_FILL_MODE>(mode);
		m_RasterizerState->Reset(m_Device, m_RasterizerDesc);
	}

	void Material::PushTexture(const Texture2DRef& texture) const
	{
		m_Textures->Push(texture);
	}

	void Material::SetSamplerState(const SamplerStateRef& samplerState)
	{
		m_SamplerState = samplerState;
	}

	void Material::RemoveTexture(const size_t index) const
	{
		m_Textures->Erase(index);
	}

	void Material::Bind() const
	{
		m_VertexShader->Bind(m_DeviceCtx);
		m_InputLayout->Bind(m_DeviceCtx);
		m_PixelShader->Bind(m_DeviceCtx);
		m_DefaultConstantBuffer->Bind(m_DeviceCtx);
		m_Textures->BindToPixelShader(m_DeviceCtx);
		m_SamplerState->Bind(m_DeviceCtx);
		m_RasterizerState->Bind(m_DeviceCtx);

		UINT startSlot = 1;
		for (auto& cb : m_ConstantBuffers)
		{
			if(cb)
			{
				cb->Bind(m_DeviceCtx, startSlot);
				++startSlot;
			}
		}
	}

	void Material::SetData(ID3D11DeviceContext* pContext, void* data, size_t index) const
	{
		m_ConstantBuffers[index]->Update(pContext, data);
	}

	size_t Material::GetNumberOfConstantBuffers(ID3DBlob* blob)
	{
		ID3D11ShaderReflection* pReflection = nullptr;
		D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection,
			reinterpret_cast<void**>(&pReflection));
		D3D11_SHADER_DESC shaderDesc{};
		pReflection->GetDesc(&shaderDesc);
		return shaderDesc.ConstantBuffers;
	}

	void Material::UpdateLightData(ID3D11DeviceContext* pContext, const PSDefaultCBufferData& data) const
	{
		m_DefaultConstantBuffer->Update(pContext, data);
	}
}
