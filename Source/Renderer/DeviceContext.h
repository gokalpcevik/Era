#pragma once
#include <d3d11_3.h>
#include <wrl/client.h>
#include <wincodec.h>
#include <DirectXTK/ScreenGrab.h>
#include "InfoQueue.h"
#include "ErrorChecker.h"

#define CONTEXT_CALL(HR); (HR); InfoQueue::Flush();

namespace Era
{
    using Microsoft::WRL::ComPtr;
    class DeviceContext
    {
        friend class Renderer;

    public:
        explicit DeviceContext(const ComPtr<ID3D11DeviceContext>& pDeviceContext);
        ~DeviceContext() = default;

        [[nodiscard]] const ComPtr<ID3D11DeviceContext>& GetD3D11DeviceContext() const { return m_pDeviceContextD3D; }
        [[nodiscard]] const ComPtr<ID3D11RenderTargetView>& GetBackBufferRTV() const { return m_pBackBufferRTV; }
    private:
        void Clear(const float* color);
        void CreateBackBufferRTV(ID3D11Device3* pDevice,IDXGISwapChain* pSwapChain);
        void CreateDepthStencilView_State(ID3D11Device3* pDevice,IDXGISwapChain* pSwapChain);
        void ResetDepthStencilView_State();
        void ResetBackBufferRTVs();
        void SetViewport(const D3D11_VIEWPORT& vp) const;
    private:
        ComPtr<ID3D11DeviceContext> m_pDeviceContextD3D = nullptr;
        ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV = nullptr;
        ComPtr<ID3D11DepthStencilView> m_pDepthSV = nullptr;
        ComPtr<ID3D11DepthStencilState> m_pDepthSS = nullptr;

    };
}