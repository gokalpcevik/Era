//
// Created by Gokalp on 12/27/2021.
//

#pragma once
#include <d3d11_3.h>
#include <cstdint>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
    using Microsoft::WRL::ComPtr;

    template<typename Vertex>
    class VertexBuffer
	{
    public:
        VertexBuffer(ID3D11Device* pDevice,Vertex* vertices,uint32_t size);

        void Bind(ID3D11DeviceContext* pContext) const;
    private:
        ComPtr<ID3D11Buffer> m_vertexBufferD3D = nullptr;
    };

    template<typename Vertex>
    VertexBuffer<Vertex>::VertexBuffer(ID3D11Device *pDevice, Vertex *vertices, uint32_t size)
    {
        if (!vertices)
            return;
        D3D11_BUFFER_DESC desc{};
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.ByteWidth = size * sizeof(Vertex);
        desc.StructureByteStride = sizeof(Vertex);
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA sbData{};
        sbData.pSysMem = vertices;
        sbData.SysMemSlicePitch = 0;
        sbData.SysMemPitch = 0;
        DX_RESULT(pDevice->CreateBuffer(&desc, &sbData, &m_vertexBufferD3D));
    }

    template <typename Vertex>
    void VertexBuffer<Vertex>::Bind(ID3D11DeviceContext* pContext) const
    {
        const UINT stride = sizeof(Vertex);
        constexpr UINT offset = 0;
        pContext->IASetVertexBuffers(0u, 1u, m_vertexBufferD3D.GetAddressOf(), &stride, &offset);
    }

    template<typename Vertex>
    using VertexBufferRef = std::shared_ptr<VertexBuffer<Vertex>>;
}


