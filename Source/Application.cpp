//
// Created by Gokalp on 12/15/2021.
//

#include "Application.h"

namespace Era
{
    int Application::Start()
    {
        Log::Init();
        uint32_t w = 1600;
        uint32_t h = 900;
        m_Window = std::make_shared<Window>(150,100,w,h,"ERA",SDL_WINDOW_RESIZABLE);
        if(m_Window->IsNullPtr()) 
            return 0;
        ERA_INFO("Created a window with the following size: {}x{}",w,h);
        m_Scene = std::make_shared<Scene>(GetRenderer());
        camera = m_Scene->CreateEntity();
        auto view = DX::XMMatrixLookAtLH(DX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f), DX::XMVectorSet(0.0f, 0.0f, 5.0f, 1.0f), DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

        camera.AddComponent<CameraComponent>(ProjectionType::Perspective, (float)m_Window->GetWidth() / (float)m_Window->GetHeight());
        camera.GetComponent<CameraComponent>().SetPerspectiveFOV(45.0f);
        camera.GetComponent<CameraComponent>().SetViewMatrix(view);
        camera.GetComponent<CameraComponent>().SetPerspectiveFar(500.0f);
        camera.GetComponent<CameraComponent>().SetPerspectiveNear(0.1f);
        camera.GetComponent<CameraComponent>().SetAspectRatio((float)m_Window->GetWidth() / (float)m_Window->GetHeight());

        box = m_Scene->CreateEntity();
        box.AddComponent<TransformComponent>().Translation = { 0.0f,0.0f,+5.0f };
        DX::XMVECTOR rot = DX::XMQuaternionRotationRollPitchYaw(45.0f,45.0f,0.0f);
        DX::XMStoreFloat4(&box.GetComponent<TransformComponent>().Rotation, rot);
        box.GetComponent<TransformComponent>().Scale = { 1.0f,1.0f,1.0f };
        box.AddComponent<MeshRendererComponent>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), "Assets/box.fbx");

        return Update();
    }

    int Application::Update() const
    {
        while (m_Running)
        {
            while (m_Window->PollEvents())
            {
                switch (m_Window->GetEvent().type)
                {
	                case SDL_QUIT:
	                {
	                    return 0;
	                }
	                case SDL_WINDOWEVENT:
	                {
	                    switch (m_Window->GetEvent().window.event)
	                    {
		                    case SDL_WINDOWEVENT_RESIZED:
		                    {
		                        m_Window->GetRenderer()->Resize();
		                        m_Scene->OnResize(m_Window->GetEvent().window.data1, m_Window->GetEvent().window.data2);
                                ERA_INFO("Window resized to {0}x{1}", m_Window->GetEvent().window.data1, m_Window->GetEvent().window.data2);
		                        break;
		                    }
	                    }
	                }
                }
            }
            m_Scene->Update();
            const float color[] = { 0.0f,0.0f,0.0f,1.0f };
            GetRenderer()->Clear(color);
            m_Scene->Draw();
            GetRenderer()->Present(1, 0);
        }
        return 0;
    }

    Application& Application::Get()
    {
        static Application S;
        return S;
    }

    void Application::Exit()
    {
        m_Running = false;
    }
}
