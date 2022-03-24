//
// Created by Gokalp on 12/22/2021.
//

#pragma once
#include <cstdint>
#include <dxgidebug.h>
#include <d3d11_4.h>
#include <dxgi1_4.h>
#include <wrl/client.h>
#include "ErrorChecker.h"

namespace Era
{
    class Window;
    using Microsoft::WRL::ComPtr;
    class SwapChain
    {
        friend class Renderer;
    public:
        SwapChain(const Window& window,const ComPtr<ID3D11Device3>& pDevice);
        ~SwapChain() = default;
        [[nodiscard]] auto GetDXGISwapChain() const -> const ComPtr<IDXGISwapChain3>& { return m_pSwapChainD3D; }
        [[nodiscard]] auto GetDXGIAdapter() const -> const ComPtr<IDXGIAdapter3>& { return pDXGIAdapter3; }
    private:
        void Present(uint32_t syncInterval,uint32_t flags) const;
        void ResizeBuffers() const;
    private:
        ComPtr<IDXGISwapChain3> m_pSwapChainD3D = nullptr;
        ComPtr<IDXGIDevice3> pDXGIDevice;
        ComPtr<IDXGIAdapter3> pDXGIAdapter3;
        ComPtr<IDXGIFactory3> pDXGIFactory;
    };
}



