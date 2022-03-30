//
// Created by Gokalp on 12/15/2021.
//

#pragma once
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
#include "Material.h"

namespace Era
{
    using Microsoft::WRL::ComPtr;
    class GraphicsDevice
    {
    public:
        GraphicsDevice();
        ~GraphicsDevice() = default;

        [[nodiscard]] auto GetDeviceContext() const -> const std::shared_ptr<DeviceContext>& { return m_pDeviceContext; }
        [[nodiscard]] auto GetD3D11Device() const -> const ComPtr<ID3D11Device>& { return m_pDeviceD3D; }
        [[nodiscard]] auto GetD3D11Device3() const -> const ComPtr<ID3D11Device3>& { return m_pDevice3D3D; }
        [[nodiscard]] auto GetD3D11DeviceContext() const -> const ComPtr<ID3D11DeviceContext>& { return m_pDeviceContextD3D; }
        template<typename T>
        [[nodiscard]] auto CreateVertexBuffer(T* vertices,uint32_t size) const -> std::shared_ptr<VertexBuffer<T>>;
        [[nodiscard]] auto CreateIndexBuffer(uint32_t* indices, uint32_t size) const -> std::shared_ptr<IndexBuffer>;
        [[nodiscard]] auto CreateTexture2D(std::filesystem::path path) const -> std::shared_ptr<Texture2D>;
        [[nodiscard]] auto CreateInputLayout(
            const D3D11_INPUT_ELEMENT_DESC* elements,
            uint32_t size,
            const void* pShaderBytecodeWithInputSignature,
            size_t byteCodeLength) const -> std::shared_ptr<InputLayout>;
        [[nodiscard]] auto CreateVertexShader(std::wstring_view path) const -> VertexShaderRef;
        [[nodiscard]] auto CreatePixelShader(std::wstring_view path) const -> PixelShaderRef;
        [[nodiscard]] auto CreateMaterial(std::filesystem::path vs, std::filesystem::path ps) const->MaterialRef;
	private:
        ComPtr<ID3D11Device> m_pDeviceD3D = nullptr;
        ComPtr<ID3D11Device3> m_pDevice3D3D = nullptr;
        ComPtr<ID3D11DeviceContext> m_pDeviceContextD3D = nullptr;
        std::shared_ptr<DeviceContext> m_pDeviceContext;
    };

    template <typename T>
    auto GraphicsDevice::CreateVertexBuffer(T* vertices, uint32_t size) const -> std::shared_ptr<VertexBuffer<T>>
    {
        return std::make_shared<VertexBuffer<T>>(m_pDevice3D3D.Get(), vertices, size);
    }
}



