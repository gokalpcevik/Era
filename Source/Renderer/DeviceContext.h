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

        [[nodiscard]] auto GetD3D11DeviceContext() const -> const ComPtr<ID3D11DeviceContext>& { return m_pDeviceContextD3D; }
        [[nodiscard]] auto GetBackBufferRTV() const -> const ComPtr<ID3D11RenderTargetView>& { return m_pBackBufferRTV; }

    private:
        void Clear(const float* color) const;
        void CreateBackBufferRTV(ID3D11Device3* pDevice,IDXGISwapChain* pSwapChain);
        void BindBackBufferRTVAndStencilView();
        void CreateDepthStencilViewAndState(ID3D11Device3* pDevice,IDXGISwapChain* pSwapChain);
        void ResetMainDepthStencilViewAndState();
        void ResetBackBufferRTVs();
        void SetViewport(const D3D11_VIEWPORT& vp) const;
        void DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) const;
    private:
        ComPtr<ID3D11DeviceContext> m_pDeviceContextD3D = nullptr;
        ComPtr<ID3D11RenderTargetView> m_pBackBufferRTV = nullptr;
        ComPtr<ID3D11DepthStencilView> m_MainDepthStencilView = nullptr;
        ComPtr<ID3D11DepthStencilView> m_ShadowMapDepthStencilView = nullptr;
        ComPtr<ID3D11DepthStencilState> m_MainDepthStencilState = nullptr;
        ComPtr<ID3D11DepthStencilState> m_ShadowMapDepthStencilState = nullptr;
    };
}