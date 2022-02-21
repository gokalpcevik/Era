//
// Created by Gokalp on 12/23/2021.
//

#pragma once
#include <d3d11_3.h>
#include <wrl/client.h>
#include "ErrorChecker.h"
#include "../Log.h"
#include <dxgidebug.h>

namespace Era
{
    using Microsoft::WRL::ComPtr;
    class InfoQueue
    {
    public:
        InfoQueue() = default;

        static void InitDevice(ID3D11Device3* pDevice);
        static void InitDXGI();
        static void Flush();
    private:
        static ComPtr<ID3D11InfoQueue> m_pInfoQueueD3D;
        static ComPtr<ID3D11Debug> m_pDebugD3D;
        static ComPtr<IDXGIDebug> m_pDXGIDebug;
        static ComPtr<IDXGIInfoQueue> m_pDXGIInfoQueue;
    };
}



