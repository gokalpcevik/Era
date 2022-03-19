#include "ConstantBuffer.h"

namespace Era
{
	DynamicConstantBuffer::DynamicConstantBuffer(ID3D11Device* pDevice, ID3DBlob* pBlob, size_t index, void* data) : m_Stride(0)
	{
		ID3D11ShaderReflection* pReflection = nullptr;
		D3DReflect(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), IID_ID3D11ShaderReflection,
			reinterpret_cast<void**>(&pReflection));
		D3D11_SHADER_DESC shaderDesc{};
		pReflection->GetDesc(&shaderDesc);
		auto type = static_cast<D3D11_SHADER_VERSION_TYPE>(D3D11_SHVER_GET_TYPE(shaderDesc.Version));
		m_Type = static_cast<ConstantBufferType>(type);

		if(shaderDesc.ConstantBuffers <= 1)
		{
			return;
		}

		ID3D11ShaderReflectionConstantBuffer* pCBReflection = pReflection->GetConstantBufferByIndex(index);
		D3D11_SHADER_BUFFER_DESC CBDesc{};
		pCBReflection->GetDesc(&CBDesc);
		m_ByteWidth = CBDesc.Size;
		m_Name = CBDesc.Name;
		

		D3D11_BUFFER_DESC Desc{};
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		Desc.ByteWidth = m_ByteWidth;
		Desc.StructureByteStride = 0;
		Desc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA SubresourceData{};
		SubresourceData.SysMemPitch = 0;
		SubresourceData.SysMemSlicePitch = 0;
		SubresourceData.pSysMem = &data;
		DX_RESULT(pDevice->CreateBuffer(&Desc, &SubresourceData, &m_ConstantBuffer));
	}

	void DynamicConstantBuffer::Bind(ID3D11DeviceContext* pContext)
	{
		switch (m_Type)
		{
		default:
		{
			pContext->VSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		case ConstantBufferType::Vertex:
		{
			pContext->VSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		case ConstantBufferType::Pixel:
		{
			pContext->PSSetConstantBuffers(0u, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		}
	}

	void DynamicConstantBuffer::Bind(ID3D11DeviceContext* pContext, UINT startSlot)
	{
		switch (m_Type)
		{
		default:
		{
			pContext->VSSetConstantBuffers(startSlot, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		case ConstantBufferType::Vertex:
		{
			pContext->VSSetConstantBuffers(startSlot, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		case ConstantBufferType::Pixel:
		{
			pContext->PSSetConstantBuffers(startSlot, 1u, m_ConstantBuffer.GetAddressOf());
			break;
		}
		}
	}

	void DynamicConstantBuffer::Update(ID3D11DeviceContext* pContext, void* data) const
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (void* dataPtr = mappedResource.pData; dataPtr)
		{
			dataPtr = data;
		}
		else
		{
			ERA_ERROR("Data Pointer was nullptr when trying to set constant buffer contents.");
		}
		pContext->Unmap(m_ConstantBuffer.Get(), 0);
	}
}
