#include "IndexBuffer.h"

namespace Era
{
	IndexBuffer::IndexBuffer(ID3D11Device* pDevice, uint32_t* indices, uint32_t size) : m_Size(size)
	{
		D3D11_BUFFER_DESC desc{};
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.MiscFlags = 0;
		desc.ByteWidth = size * sizeof(uint32_t);
		desc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA sbData{};
		sbData.pSysMem = indices;
		sbData.SysMemPitch = 0;
		sbData.SysMemSlicePitch = 0;

		DX_RESULT(pDevice->CreateBuffer(&desc, &sbData, &m_indexBufferD3D));
	}

	void IndexBuffer::Bind(ID3D11DeviceContext* pContext) const
	{
		pContext->IASetIndexBuffer(m_indexBufferD3D.Get(), DXGI_FORMAT_R32_UINT,0u);
	}
}
