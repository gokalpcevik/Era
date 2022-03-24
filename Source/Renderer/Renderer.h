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
#include "../Scene/MeshRendererComponent.h"
#include "Material.h"
#include "../Core/Window.h"


namespace Era
{

    class Window;

    using Microsoft::WRL::ComPtr;
    class Renderer : public IWindowListener
    {
    public:
        Renderer(Window& window);

        void Clear(const float colorRGBA[]) const;
        void Present(uint32_t syncInterval,uint32_t flags);
        void SetViewport(const D3D11_VIEWPORT& vp) const;
        void TakeScreenShot(const std::wstring& file) const;
        void DrawMesh(const MeshRendererComponent& mrc);
        auto GetWindow() const -> Window * { return m_pWindow; }

        uint64_t GetDrawCallCount() const { return m_DrawCalls; }

        [[nodiscard]] auto GetGraphicsDevice() const -> const std::shared_ptr<GraphicsDevice>& { return m_Device; }
        [[nodiscard]] auto GetSwapChain() const -> const std::shared_ptr<SwapChain>& { return m_SwapChain; }

    private:
        void OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height) override;

    private:
        std::shared_ptr<GraphicsDevice> m_Device;
        std::shared_ptr<DeviceContext> m_DeviceContext;
        std::shared_ptr<SwapChain> m_SwapChain;
        uint64_t m_DrawCalls = 0;
        Window* m_pWindow = nullptr;
        DX::XMMATRIX m_ViewProjection{};

        friend class Scene;
    };
}



