//
// Created by Gokalp on 12/15/2021.
//

#pragma once
#include <d3d11.h>
#include <d3d11_3.h>
#include <cstdint>
#include <memory>
#include <wrl/client.h>
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "InputLayout.h"
#include "Shader.h"
#include "ErrorChecker.h"


namespace Era
{
    using Microsoft::WRL::ComPtr;
    class GraphicsDevice
    {
    public:
        GraphicsDevice();
        ~GraphicsDevice() = default;

        [[nodiscard]] const std::shared_ptr<DeviceContext>& GetDeviceContext() const { return m_pDeviceContext; }
        [[nodiscard]] const ComPtr<ID3D11Device>& GetD3D11Device() const { return m_pDeviceD3D; }
        [[nodiscard]] const ComPtr<ID3D11Device3>& GetD3D11Device3() const { return m_pDevice3D3D; }
        [[nodiscard]] const ComPtr<ID3D11DeviceContext>& GetD3D11DeviceContext() const { return m_pDeviceContextD3D; }
        template<typename T>
        [[nodiscard]] std::shared_ptr<VertexBuffer<T>> CreateVertexBuffer(T* vertices,uint32_t size) const;
        [[nodiscard]] std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t size) const;
        [[nodiscard]] std::shared_ptr<Texture2D> CreateTexture2D(std::wstring_view path) const;
        [[nodiscard]] std::shared_ptr<InputLayout> CreateInputLayout(
            const D3D11_INPUT_ELEMENT_DESC* elements,
            uint32_t size,
            const void* pShaderBytecodeWithInputSignature,
            size_t byteCodeLength) const;
        [[nodiscard]] VertexShaderRef CreateVertexShader(std::wstring_view path) const;
        [[nodiscard]] PixelShaderRef CreatePixelShader(std::wstring_view path) const;
	private:
        ComPtr<ID3D11Device> m_pDeviceD3D = nullptr;
        ComPtr<ID3D11Device3> m_pDevice3D3D = nullptr;
        ComPtr<ID3D11DeviceContext> m_pDeviceContextD3D = nullptr;
        std::shared_ptr<DeviceContext> m_pDeviceContext;
    };

    template <typename T>
    std::shared_ptr<VertexBuffer<T>> GraphicsDevice::CreateVertexBuffer(T* vertices, uint32_t size) const
    {
        return std::make_shared<VertexBuffer<T>>(m_pDevice3D3D.Get(), vertices, size);
    }
}



