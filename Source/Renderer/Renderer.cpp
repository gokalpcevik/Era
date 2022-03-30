//
// Created by Gokalp on 12/23/2021.
//

#include "Renderer.h"


namespace Era
{
    Renderer::Renderer(Window& window) :
	m_Device(std::make_shared<GraphicsDevice>()),
	m_DeviceContext(m_Device->GetDeviceContext()),
	m_SwapChain(std::make_shared<SwapChain>(window, m_Device->GetD3D11Device3())),
	m_pWindow(&window)
    {
        InfoQueue::InitDevice(m_Device->GetD3D11Device3().Get());
        InfoQueue::InitDXGI();
        m_DeviceContext->CreateBackBufferRTV(m_Device->GetD3D11Device3().Get(),m_SwapChain->GetDXGISwapChain().Get());
        m_DeviceContext->CreateDepthStencilViewAndState(m_Device->GetD3D11Device3().Get(), m_SwapChain->GetDXGISwapChain().Get());
        D3D11_VIEWPORT vp{};
        vp.Height = static_cast<float>(m_pWindow->GetHeight());
        vp.Width = static_cast<float>(m_pWindow->GetWidth());
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        SetViewport(vp);
        window.Subscribe(this);
    }

    void Renderer::Clear(const float colorRGBA[]) const
    {
	    m_DeviceContext->BindBackBufferRTVAndStencilView();
	    m_DeviceContext->Clear(colorRGBA);
    }

    void Renderer::Present(uint32_t syncInterval, uint32_t flags)
    {
        m_SwapChain->Present(syncInterval,flags);
        InfoQueue::Flush();
        m_DrawCalls = 0;
    }

    void Renderer::SetViewport(const D3D11_VIEWPORT& vp) const
    {
        m_DeviceContext->SetViewport(vp);
    }

    void Renderer::DrawMesh(const MeshRendererComponent& mrc)
    {
        mrc.GetMaterial()->UnbindTextures();
	    ID3D11DeviceContext* pContext = m_DeviceContext->GetD3D11DeviceContext().Get();
        mrc.GetVertexBuffer()->Bind(pContext);
        mrc.GetIndexBuffer()->Bind(pContext);
	    mrc.GetVSConstantBuffer()->Bind(pContext);
        mrc.GetMaterial()->Bind();
        pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_DeviceContext->DrawIndexed(mrc.GetIndexBuffer()->GetCount(), 0, 0);
        m_DrawCalls++;
    }

    void Renderer::OnWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height)
    {
        m_DeviceContext->ResetBackBufferRTVs();
        m_SwapChain->ResizeBuffers();
        m_DeviceContext->CreateDepthStencilViewAndState(m_Device->GetD3D11Device3().Get(), m_SwapChain->GetDXGISwapChain().Get());
        m_DeviceContext->CreateBackBufferRTV(m_Device->GetD3D11Device3().Get(), m_SwapChain->GetDXGISwapChain().Get());
        D3D11_VIEWPORT vp{};
        vp.Height = static_cast<float>(m_pWindow->GetHeight());
        vp.Width = static_cast<float>(m_pWindow->GetWidth());
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        SetViewport(vp);
    }


    void Renderer::TakeScreenShot(const std::wstring& file) const
    {
        ID3D11Resource* p_rsc;
        m_DeviceContext->GetBackBufferRTV()->GetResource(&p_rsc);
        DirectX::SaveWICTextureToFile(m_DeviceContext->GetD3D11DeviceContext().Get(), p_rsc, GUID_ContainerFormatTiff, file.c_str());
        p_rsc->Release();
    }
}

