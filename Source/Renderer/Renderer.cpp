//
// Created by Gokalp on 12/23/2021.
//

#include "Renderer.h"
#include "../Window.h"

namespace Era
{
    Renderer::Renderer(const Window& window) :
	m_Device(std::make_shared<GraphicsDevice>()),
	m_DeviceContext(m_Device->GetDeviceContext()),
	m_SwapChain(std::make_shared<SwapChain>(window, m_Device->GetD3D11Device3())),
	m_pWindow(&window)
    {
        InfoQueue::InitDevice(m_Device->GetD3D11Device3().Get());
        InfoQueue::InitDXGI();
        m_DeviceContext->CreateBackBufferRTV(m_Device->GetD3D11Device3().Get(),m_SwapChain->GetDXGISwapChain().Get());
        m_DeviceContext->CreateDepthStencilView_State(m_Device->GetD3D11Device3().Get(), m_SwapChain->GetDXGISwapChain().Get());
    }

    void Renderer::Clear(const float colorRGBA[]) const
    {
        m_DeviceContext->Clear(colorRGBA);
    }

    void Renderer::Present(uint32_t syncInterval, uint32_t flags) const
    {
        m_SwapChain->Present(syncInterval,flags);
        InfoQueue::Flush();
    }

    void Renderer::Resize() const
    {
	    m_DeviceContext->ResetBackBufferRTVs();
	    m_SwapChain->ResizeBuffers();
	    m_DeviceContext->CreateDepthStencilView_State(m_Device->GetD3D11Device3().Get(), m_SwapChain->GetDXGISwapChain().Get());
	    m_DeviceContext->CreateBackBufferRTV(m_Device->GetD3D11Device3().Get(),m_SwapChain->GetDXGISwapChain().Get());
        D3D11_VIEWPORT vp{};
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Height = m_pWindow->GetHeight();
        vp.Width = m_pWindow->GetWidth();
        m_DeviceContext->SetViewport(vp);
    }

    void Renderer::SetViewport(const D3D11_VIEWPORT& vp) const
    {
        m_DeviceContext->SetViewport(vp);
    }

    void Renderer::TakeScreenShot(const std::wstring& file) const
    {
        ID3D11Resource* p_rsc;
        m_DeviceContext->GetBackBufferRTV()->GetResource(&p_rsc);
        DirectX::SaveWICTextureToFile(m_DeviceContext->GetD3D11DeviceContext().Get(), p_rsc, GUID_ContainerFormatTiff, file.c_str());
        p_rsc->Release();
    }

    void Renderer::DrawMesh(const DX::XMMATRIX& transform, const MeshRendererComponent& mrc) const
    {
        mrc.GetVertexBuffer()->Bind(m_DeviceContext->GetD3D11DeviceContext().Get());
        mrc.GetIndexBuffer()->Bind(m_DeviceContext->GetD3D11DeviceContext().Get());
        mrc.GetPixelShader()->Bind(m_DeviceContext->GetD3D11DeviceContext().Get());
        mrc.GetVertexShader()->Bind(m_DeviceContext->GetD3D11DeviceContext().Get());
        mrc.GetInputLayout()->Bind(m_DeviceContext->GetD3D11DeviceContext().Get());
        mrc.SetPrimitiveTopology(m_DeviceContext->GetD3D11DeviceContext().Get());
    }
}

