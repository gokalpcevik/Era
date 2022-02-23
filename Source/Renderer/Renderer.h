//
// Created by Gokalp on 12/23/2021.
//

#pragma once
#include <wrl/client.h>
#include <DirectXTK/WICTextureLoader.h>
#include "GraphicsDevice.h"
#include "DeviceContext.h"
#include "SwapChain.h"
#include "InfoQueue.h"
#include "../Scene/Components.h"

namespace Era
{
    class Window;
    using Microsoft::WRL::ComPtr;
    class Renderer
    {
    public:
        Renderer(const Window& window);

        void Clear(const float colorRGBA[]) const;
        void Present(uint32_t syncInterval,uint32_t flags) const;
        void Resize() const;
        void SetViewport(const D3D11_VIEWPORT& vp) const;
        void TakeScreenShot(const std::wstring& file) const;
        void DrawMesh(const MeshRendererComponent& mrc) const;

        const std::shared_ptr<GraphicsDevice>& GetGraphicsDevice() const { return m_Device; }

    private:
        std::shared_ptr<GraphicsDevice> m_Device;
        std::shared_ptr<DeviceContext> m_DeviceContext;
        std::shared_ptr<SwapChain> m_SwapChain;

        Window const* m_pWindow = nullptr;
        DX::XMMATRIX m_ViewProjection{};

        friend class Scene;
    };
}



