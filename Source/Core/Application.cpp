//
// Created by Gokalp on 12/15/2021.
//

#include "Application.h"

namespace Era
{
	auto Application::Start() -> int
	{
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        Log::Init();
        uint32_t w = 1600;
        uint32_t h = 900;
        m_Window = std::make_shared<Window>(150,100,w,h,"ERA",SDL_WINDOW_RESIZABLE);
        if(m_Window->IsNullPtr()) 
            return 0;
        ERA_INFO("Created a window with the following size: {}x{}",w,h);
        m_Scene = std::make_shared<Scene>(GetRenderer());
        camera = m_Scene->CreateEntity();
        auto view = DX::XMMatrixLookAtLH(DX::XMVectorSet(0.0f, 0.0f, +1.0f, 1.0f), DX::XMVectorSet(0.0f, 0.0f, 5.0f, 1.0f), DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

        auto&& cc = camera.AddComponent<CameraComponent>(ProjectionType::Perspective, (float)m_Window->GetWidth() / (float)m_Window->GetHeight());
		cc.SetPerspectiveHalfAngleFOV(45.0f);
        cc.SetViewMatrix(view);
        cc.SetPerspectiveFar(500.0f);
        cc.SetPerspectiveNear(0.1f);
        cc.SetAspectRatio((float)m_Window->GetWidth() / (float)m_Window->GetHeight());

        directionalLight = m_Scene->CreateEntity();
        auto&& dlc = directionalLight.AddComponent<DirectionalLightComponent>();
        dlc.LightDirection = { -0.1f,-1.0f,0.0f,1.0f };
        dlc.SpecularLightColor = { 0.9f,0.9f,0.9f,1.0f };
        dlc.DiffuseLightColor = { 0.4f,0.1f,0.1f,1.0f };
        dlc.AmbientLightColor = { 0.08f,0.08f,0.08f,1.0f };
        dlc.Shininess = 50.0f;
		box = m_Scene->CreateEntity();
        box.AddComponent<TransformComponent>().Translation = { 0.0f,0.0f,+5.0f };
        DX::XMVECTOR rot = DX::XMQuaternionRotationRollPitchYaw(45.0f,0.0f,0.0f);
        DX::XMStoreFloat4(&box.GetComponent<TransformComponent>().Rotation, rot);
        box.GetComponent<TransformComponent>().Scale = { 1.0f,1.0f,1.0f };

        box2 = m_Scene->CreateEntity();
        box2.AddComponent<TransformComponent>().Translation = { 1.5f,0.0f,+5.0f };
        DX::XMStoreFloat4(&box2.GetComponent<TransformComponent>().Rotation, rot);
        box2.GetComponent<TransformComponent>().Scale = { 1.0f,1.0f,1.0f };

        //CreateMeshes();
        m_CreateMeshesFuture = std::async(std::launch::async,&Application::CreateMeshes,this);

        return Update();
    }

	auto Application::Update() -> int
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
                                ERA_INFO("Window resized to {0}x{1}", m_Window->GetEvent().window.data1, m_Window->GetEvent().window.data2);
		                        break;
		                    }
	                    }
	                }
                default: break;
                }
            }
            m_Scene->Update();
            static float x = 0.01f;
            x += 0.01f;
            xDir = std::cos(x);
            directionalLight.GetComponent<DirectionalLightComponent>().LightDirection = { xDir,-0.75f,0.0f,1.0f };

            constexpr float color[] = { 0.1f,0.1f,0.1f,1.0f };
            GetRenderer()->Clear(color);
            m_Scene->Draw();
            GetRenderer()->Present(1, 0);
        }
        return 0;
    }

	void Application::CreateMeshes()
	{
        std::lock_guard lock(m_CreateMeshesMutex);
        MeshAsset meshAsset("Assets/monkey.fbx", 0);
        meshAsset.Import();
        box.AddComponent<MeshRendererComponent>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), meshAsset);
	}

	auto Application::Get() -> Application&
	{
        static Application S;
        return S;
    }

    void Application::Exit()
    {
        m_Running = false;
    }
}
