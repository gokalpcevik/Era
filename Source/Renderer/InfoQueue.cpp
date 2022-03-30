//
// Created by Gokalp on 12/23/2021.
//

#include "InfoQueue.h"

namespace Era
{
    ComPtr<ID3D11Debug> InfoQueue::m_pDebugD3D;
    ComPtr<ID3D11InfoQueue> InfoQueue::m_pInfoQueueD3D;
    ComPtr<IDXGIDebug> InfoQueue::m_pDXGIDebug;
    ComPtr<IDXGIInfoQueue> InfoQueue::m_pDXGIInfoQueue;

    void InfoQueue::InitDevice(ID3D11Device3 *pDevice)
    {
        assert(pDevice != nullptr);
        auto dbgResult = pDevice->QueryInterface(__uuidof(ID3D11Debug), &m_pDebugD3D);
        if (SUCCEEDED(dbgResult))
        {
            auto infoQueueResult = pDevice->QueryInterface(__uuidof(ID3D11InfoQueue), &m_pInfoQueueD3D);
            if (SUCCEEDED(infoQueueResult))
            {
                m_pInfoQueueD3D->ClearStoredMessages();
                m_pInfoQueueD3D->ClearRetrievalFilter();
                m_pInfoQueueD3D->ClearStorageFilter();
                m_pInfoQueueD3D->PushEmptyStorageFilter();
                m_pInfoQueueD3D->PushEmptyRetrievalFilter();
            }
            else
                ERA_ERROR("Error while initializing ID3D11InfoQueue in file: {}", __FILE__);
        }
        else
            ERA_ERROR("Error while initializing ID3D11Debug in file: {}", __FILE__);
    }

    void InfoQueue::InitDXGI()
    {
        typedef HRESULT(WINAPI* LPDXGIGETDEBUGINTERFACE)(REFIID, void**);

        HMODULE dxgidebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (dxgidebug)
        {
            auto const dxgiGetDebugInterface = reinterpret_cast<LPDXGIGETDEBUGINTERFACE>(
                reinterpret_cast<void*>(GetProcAddress(dxgidebug, "DXGIGetDebugInterface")));

            if (SUCCEEDED(dxgiGetDebugInterface(IID_PPV_ARGS(m_pDXGIInfoQueue.GetAddressOf()))))
            {
                m_pDXGIInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
                m_pDXGIInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);
                m_pDXGIInfoQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
                m_pDXGIInfoQueue->ClearRetrievalFilter(DXGI_DEBUG_ALL);
                m_pDXGIInfoQueue->ClearStorageFilter(DXGI_DEBUG_ALL);
                m_pDXGIInfoQueue->PushEmptyStorageFilter(DXGI_DEBUG_ALL);
                m_pDXGIInfoQueue->PushEmptyRetrievalFilter(DXGI_DEBUG_ALL);
            }
        }
    }

    void InfoQueue::Flush()
    {
	    const auto numStoredMessages = m_pInfoQueueD3D->GetNumStoredMessages();

        for (int i = 0; i < numStoredMessages; ++i)
        {
            SIZE_T msgLen;
            if (SUCCEEDED(m_pInfoQueueD3D->GetMessage(i, nullptr, &msgLen)))
            {
                auto* msg = static_cast<D3D11_MESSAGE*>(malloc(msgLen));
                m_pInfoQueueD3D->GetMessage(i, msg, &msgLen);

                switch (msg->Severity)
                {
                case D3D11_MESSAGE_SEVERITY_CORRUPTION:
                {
                    ERA_ERROR("CORRUPTION\n");
                    ERA_ERROR("{0} \n", msg->pDescription);
                    break;
                }
                case D3D11_MESSAGE_SEVERITY_ERROR:
                {
                    ERA_ERROR("{0} \n", msg->pDescription);
                    break;
                }
                case D3D11_MESSAGE_SEVERITY_WARNING:
                {
                    ERA_WARN("{0} \n", msg->pDescription);
                    break;
                }
                case D3D11_MESSAGE_SEVERITY_INFO:
                {
                    //ERA_INFO("{0} \n", msg->pDescription);
                    break;
                }
                case D3D11_MESSAGE_SEVERITY_MESSAGE:
	                {
                    ERA_INFO("{0} \n", msg->pDescription);
                    break;
	                }
                default:
                    break;

                }
                free(msg);
            }
            else
                ERA_ERROR("Error while getting info queue message in file {}", __FILE__);
        }
        m_pInfoQueueD3D->ClearStoredMessages();


        const auto numStoredMessagesDXGI = m_pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);

        for (int i = 0; i < numStoredMessagesDXGI; ++i)
        {
            SIZE_T msgLen;
            if (SUCCEEDED(m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLen)))
            {
                auto* msg = static_cast<DXGI_INFO_QUEUE_MESSAGE*>(malloc(msgLen));
                m_pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, msg, &msgLen);

                switch (msg->Severity)
                {
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION:
                {
                    ERA_ERROR("DXGI CORRUPTION\n");
                    ERA_ERROR("{0} \n", msg->pDescription);
                    break;
                }
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR:
                {
                    ERA_ERROR("{0} \n", msg->pDescription);
                    break;
                }
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING:
                {
                    //ERA_WARN("{0} \n", msg->pDescription);
                    break;
                }
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_INFO:
                    break;
                case DXGI_INFO_QUEUE_MESSAGE_SEVERITY_MESSAGE:
                    break;
                }
                free(msg);
            }
            else
                ERA_ERROR("Error while getting info queue message in file {}", __FILE__);
        }
        m_pDXGIInfoQueue->ClearStoredMessages(DXGI_DEBUG_ALL);
    }
}
