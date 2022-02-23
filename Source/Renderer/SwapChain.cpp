//
// Created by Gokalp on 12/22/2021.
//

#include "SwapChain.h"
#include "../Window.h"

namespace Era
{
    SwapChain::SwapChain(const Window& window, const ComPtr<ID3D11Device3>& pDevice)
    {
        DXGI_SWAP_CHAIN_DESC SwapChainDesc{};
        SwapChainDesc.BufferDesc.Width = window.GetWidth();
        SwapChainDesc.BufferDesc.Height = window.GetHeight();
        SwapChainDesc.OutputWindow = window.GetWin32WindowHandle();
        SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
        SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        SwapChainDesc.SampleDesc.Count = 1;
        SwapChainDesc.SampleDesc.Quality = 0;
        SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        SwapChainDesc.BufferCount = 2;
        SwapChainDesc.Windowed = TRUE;
        SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        SwapChainDesc.Flags = 0;

        ComPtr<IDXGIDevice1> pDXGIDevice;
        ComPtr<IDXGIAdapter> pDXGIAdapter;
        ComPtr<IDXGIFactory1> pDXGIFactory;
        DX_RESULT(pDevice.As(&pDXGIDevice));
        DX_RESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory1), &pDXGIFactory));
        DX_RESULT(pDXGIFactory->EnumAdapters(0, &pDXGIAdapter));
        ERA_INFO("Getting the first adapter from IDXGIFactory:");
        DXGI_ADAPTER_DESC AdapterDesc;
        DX_RESULT(pDXGIAdapter->GetDesc(&AdapterDesc));

        constexpr size_t outputSize = _countof(AdapterDesc.Description) + 1; // +1 for null terminator
        char* adapterDescriptionPtr = new char[outputSize];
        size_t charsConverted = 0;
        const wchar_t* inputW = AdapterDesc.Description;
        wcstombs_s(&charsConverted, adapterDescriptionPtr, outputSize, inputW, 128);

        ERA_INFO("Adapter Description: {0}", adapterDescriptionPtr);
        delete[] adapterDescriptionPtr;
    	ERA_INFO("Dedicated Video Memory: {0}MB", (size_t)AdapterDesc.DedicatedVideoMemory/1024/1024);
    	DX_RESULT(pDXGIFactory->CreateSwapChain(pDevice.Get(),&SwapChainDesc,&m_pSwapChainD3D));
    }

    void SwapChain::Present(uint32_t syncInterval, uint32_t flags) const
    {
        if(m_pSwapChainD3D)
        {
	        if(HRESULT hr = m_pSwapChainD3D->Present(syncInterval,flags); hr == DXGI_ERROR_DEVICE_REMOVED)
            {
                ERA_ERROR("Error while IDXGISwapChain::Present : DXGI_ERROR_DEVICE_REMOVED");
            }
            else if (hr == DXGI_ERROR_DEVICE_RESET)
            {
                ERA_ERROR("Error while IDXGISwapChain::Present : DXGI_ERROR_DEVICE_RESET");
            }
        }
    }

    void SwapChain::ResizeBuffers() const
    {
        DX_RESULT(m_pSwapChainD3D->ResizeBuffers(0,0,0,DXGI_FORMAT_UNKNOWN,0));
    }
}