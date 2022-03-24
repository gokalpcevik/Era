//
// Created by Gokalp on 12/22/2021.
//

#include "SwapChain.h"
#include "../Core/Window.h"

namespace Era
{
    SwapChain::SwapChain(const Window& window, const ComPtr<ID3D11Device3>& pDevice)
    {
        DX_RESULT(pDevice.As(&pDXGIDevice));
        DX_RESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory3), &pDXGIFactory));
        DX_RESULT(pDXGIFactory->EnumAdapters(0, reinterpret_cast<IDXGIAdapter**>(pDXGIAdapter3.ReleaseAndGetAddressOf())));
        ERA_INFO("Getting the first adapter from IDXGIFactory:");
        DXGI_ADAPTER_DESC AdapterDesc;
        DX_RESULT(pDXGIAdapter3->GetDesc(&AdapterDesc));

        constexpr size_t outputSize = _countof(AdapterDesc.Description) + 1; // +1 for null terminator
        char* adapterDescriptionPtr = new char[outputSize];
        size_t charsConverted = 0;
        const wchar_t* inputW = AdapterDesc.Description;
        wcstombs_s(&charsConverted, adapterDescriptionPtr, outputSize, inputW, 128);

        ERA_INFO("Adapter Description: {0}", adapterDescriptionPtr);
        delete[] adapterDescriptionPtr;
    	ERA_INFO("Dedicated Video Memory: {0}MB", (size_t)AdapterDesc.DedicatedVideoMemory/1024/1024);

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = window.GetWidth();
        swapChainDesc.Height = window.GetHeight();
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Stereo = FALSE;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        swapChainDesc.Flags = 0;
	
        DX_RESULT(pDXGIFactory->CreateSwapChainForHwnd(pDevice.Get(), window.GetWin32WindowHandle(), &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)m_pSwapChainD3D.ReleaseAndGetAddressOf()));
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