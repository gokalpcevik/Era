#include "DeviceContext.h"


namespace Era
{
    DeviceContext::DeviceContext(const ComPtr<ID3D11DeviceContext>& pDeviceContext) : m_pDeviceContextD3D(pDeviceContext)
    {
    }

    void DeviceContext::CreateBackBufferRTV(ID3D11Device3* pDevice,IDXGISwapChain* pSwapChain)
    {
        ComPtr<ID3D11Resource> pBackBufferRsc;
        DX_RESULT(pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBufferRsc));
        DX_RESULT(pDevice->CreateRenderTargetView(pBackBufferRsc.Get(), nullptr, &m_pBackBufferRTV));
    }

    void DeviceContext::CreateDepthStencilView_State(ID3D11Device3* pDevice, IDXGISwapChain* pSwapChain)
    {
		ComPtr<ID3D11Texture2D> pDepthStencilTexture2D;

		DXGI_SWAP_CHAIN_DESC desc;
		DX_RESULT(pSwapChain->GetDesc(&desc));
		D3D11_TEXTURE2D_DESC descDepth;
		descDepth.Width = desc.BufferDesc.Width;
		descDepth.Height = desc.BufferDesc.Height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		DX_RESULT(pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencilTexture2D));


		D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;

		DepthStencilDesc.DepthEnable = true;
		DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

		DepthStencilDesc.StencilEnable = true;
		DepthStencilDesc.StencilReadMask = 0xFF;
		DepthStencilDesc.StencilWriteMask = 0xFF;

		DepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		DepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		DepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		DX_RESULT(pDevice->CreateDepthStencilState(&DepthStencilDesc, &m_pDepthSS));

		D3D11_DEPTH_STENCIL_VIEW_DESC DepthStencilViewDesc{};
		DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		DepthStencilViewDesc.Texture2D.MipSlice = 0u;

		DX_RESULT(pDevice->CreateDepthStencilView(pDepthStencilTexture2D.Get(), &DepthStencilViewDesc, &m_pDepthSV));
		
    }

    void DeviceContext::ResetDepthStencilView_State()
    {
		m_pDepthSV.Reset();
		m_pDepthSS.Reset();
    }

    void DeviceContext::Clear(const float* color)
    {
		m_pDeviceContextD3D->OMSetRenderTargets(1u, m_pBackBufferRTV.GetAddressOf(), m_pDepthSV.Get());
        m_pDeviceContextD3D->ClearRenderTargetView(m_pBackBufferRTV.Get(),color);
		m_pDeviceContextD3D->ClearDepthStencilView(m_pDepthSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    }

    void DeviceContext::ResetBackBufferRTVs()
    {
        m_pDeviceContextD3D->OMSetRenderTargets(0,nullptr,nullptr);
        m_pBackBufferRTV.Reset();
    }

    void DeviceContext::SetViewport(const D3D11_VIEWPORT &vp) const
    {
        m_pDeviceContextD3D->RSSetViewports(1u,&vp);
    }

    void DeviceContext::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) const
    {
		m_pDeviceContextD3D->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    }
}
