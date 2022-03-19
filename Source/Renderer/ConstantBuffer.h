#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "ErrorChecker.h"
#include "Shader.h"

namespace Era
{
	using Microsoft::WRL::ComPtr;


	// Comfort D3D11_SHADER_VERSION_TYPE so conversion is easier
	enum class ConstantBufferType
	{
		Unknown = -1,
		Vertex = 1,
		Pixel = 0,
		Compute = 5,
		Geometry = 2,
		Hull = 3,
		Domain = 4
	};

	template<typename Class>
	class ConstantBuffer
	{
	public:
		ConstantBuffer(ID3D11Device* pDevice, const Class& cb,ConstantBufferType type);
		void Bind(ID3D11DeviceContext* pContext) const;
		void Update(ID3D11DeviceContext* pContext, const Class& data);
	private:
		ComPtr<ID3D11Buffer> m_ConstantBuffer = nullptr;
		ConstantBufferType m_Type{};
	};

	// I very much would like this system to work dynamically but i have to do more careful planning before attempting this again
	
	class DynamicConstantBuffer
	{
	public:
		DynamicConstantBuffer(ID3D11Device* pDevice, ID3DBlob* pBlob,size_t index,void* data);
		void Bind(ID3D11DeviceContext* pContext);
		void Bind(ID3D11DeviceContext* pContext,UINT startSlot);
		void Update(ID3D11DeviceContext* pContext, void* data) const;
	private:
		uint32_t m_ByteWidth = 0;
		uint32_t m_Stride = 0;
		std::string m_Name{};
		ComPtr<ID3D11Buffer> m_ConstantBuffer = nullptr;
		ConstantBufferType m_Type{};
	};

	template<typename Class>
	ConstantBuffer<Class>::ConstantBuffer(ID3D11Device* pDevice, const Class& cb, ConstantBufferType type) : m_Type(type)
	{
		D3D11_BUFFER_DESC Desc{};
		Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		Desc.ByteWidth = sizeof(Class);
		Desc.StructureByteStride = 0;
		Desc.Usage = D3D11_USAGE_DYNAMIC;

		D3D11_SUBRESOURCE_DATA SubresourceData{};
		SubresourceData.SysMemPitch = 0;
		SubresourceData.SysMemSlicePitch = 0;
		SubresourceData.pSysMem = &cb;
		DX_RESULT(pDevice->CreateBuffer(&Desc, &SubresourceData, &m_ConstantBuffer));
	}

	template <typename Class>
	void ConstantBuffer<Class>::Bind(ID3D11DeviceContext* pContext) const
	{
		switch(m_Type)
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

	template <typename Class>
	void ConstantBuffer<Class>::Update(ID3D11DeviceContext* pContext, const Class& data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		pContext->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (Class* dataPtr = static_cast<Class*>(mappedResource.pData); dataPtr)
		{
			*dataPtr = data;
		}
		else
		{
			ERA_ERROR("Data Pointer was nullptr when trying to set constant buffer contents.");
		}
		pContext->Unmap(m_ConstantBuffer.Get(), 0);
	}


	template <typename Class>
	using ConstantBufferRef = std::shared_ptr<ConstantBuffer<Class>>;
	using DynamicConstantBufferRef = std::shared_ptr<DynamicConstantBuffer>;
}
