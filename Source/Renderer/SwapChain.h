//
// Created by Gokalp on 12/22/2021.
//

#pragma once
#include <cstdint>
#include <dxgidebug.h>
#include <d3d11_3.h>
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
        [[nodiscard]] const ComPtr<IDXGISwapChain>& GetDXGISwapChain() const { return m_pSwapChainD3D; }
    private:
        void Present(uint32_t syncInterval,uint32_t flags) const;
        void ResizeBuffers() const;
    private:
        ComPtr<IDXGISwapChain> m_pSwapChainD3D = nullptr;
    };
}



