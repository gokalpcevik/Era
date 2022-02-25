//
// Created by Gokalp on 12/15/2021.
//

#include "GraphicsDevice.h"

namespace Era
{

    GraphicsDevice::GraphicsDevice()
    {
        D3D_FEATURE_LEVEL FeatureLevels[] = {
        		D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
                D3D_FEATURE_LEVEL_10_1,
                D3D_FEATURE_LEVEL_10_0,
                D3D_FEATURE_LEVEL_9_3,
                D3D_FEATURE_LEVEL_9_2,
                D3D_FEATURE_LEVEL_9_1
        };
        auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG|D3D11_CREATE_DEVICE_BGRA_SUPPORT, FeatureLevels,
                                    _countof(FeatureLevels), D3D11_SDK_VERSION, &m_pDeviceD3D, nullptr, &m_pDeviceContextD3D
        );
        m_pDeviceD3D->QueryInterface(__uuidof(ID3D11Device3),&m_pDevice3D3D);
        if(m_pDeviceContextD3D)
                m_pDeviceContext = std::make_shared<DeviceContext>(m_pDeviceContextD3D);
    }

    auto GraphicsDevice::CreateIndexBuffer(uint32_t* indices, uint32_t size) const -> std::shared_ptr<IndexBuffer>
    {
        return std::make_shared<IndexBuffer>(m_pDevice3D3D.Get(), indices, size);
    }

    auto GraphicsDevice::CreateTexture2D(std::wstring_view path) const -> std::shared_ptr<Texture2D>
    {
        return std::make_shared<Texture2D>(m_pDevice3D3D.Get(), path);
    }

    auto GraphicsDevice::CreateInputLayout(
	    const D3D11_INPUT_ELEMENT_DESC* elements, uint32_t size, const void* pShaderBytecodeWithInputSignature,
	    size_t byteCodeLength) const -> std::shared_ptr<InputLayout>
    {
        return std::make_shared<InputLayout>(m_pDevice3D3D.Get(),
            elements,
            size,
            pShaderBytecodeWithInputSignature,
            byteCodeLength);
    }

    auto GraphicsDevice::CreateVertexShader(std::wstring_view path) const -> VertexShaderRef
    {
        return std::make_shared<VertexShader>(m_pDevice3D3D.Get(), path);
    }

    auto GraphicsDevice::CreatePixelShader(std::wstring_view path) const -> PixelShaderRef
    {
        return std::make_shared<PixelShader>(m_pDevice3D3D.Get(), path);
    }
}
