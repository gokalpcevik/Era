//
// Created by Gokalp on 12/27/2021.
//

#include "VertexBuffer.h"

namespace Era
{
	DynamicVertexBuffer::DynamicVertexBuffer(ID3D11Device* pDevice, const VertexLayout& layout, void const* data, uint32_t size) : m_Layout(layout)
	{
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.ByteWidth = size * layout.GetStride();
        desc.StructureByteStride = layout.GetStride();
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sbData{};
        sbData.pSysMem = data;
        sbData.SysMemSlicePitch = 0;
        sbData.SysMemPitch = 0;
        DX_RESULT(pDevice->CreateBuffer(&desc, &sbData, &m_vertexBufferD3D));
	}

	void DynamicVertexBuffer::Bind(ID3D11DeviceContext* pContext)
	{
        const UINT stride = m_Layout.GetStride();
        const UINT offset = 0u;
        pContext->IASetVertexBuffers(0u, 1u, m_vertexBufferD3D.GetAddressOf(), &stride, &offset);
	}
}

