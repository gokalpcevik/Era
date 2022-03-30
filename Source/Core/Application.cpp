//
// Created by Gokalp on 12/15/2021.
//

#include "Application.h"

namespace Era
{
	auto Application::Start() -> int
	{
        Log::Init();
        uint32_t w = 1600;
        uint32_t h = 900;
        m_Window = std::make_shared<Window>(150,100,w,h,"ERA",SDL_WINDOW_RESIZABLE);
        if(m_Window->IsNullPtr()) 
            return 0;
        ERA_INFO("Created a window with the following size: {}x{}",w,h);
        m_Scene = std::make_shared<Scene>(GetRenderer());
        m_Camera = m_Scene->CreateEntity();
        auto view = DX::XMMatrixLookAtLH(DX::XMVectorSet(0.0f, 0.0f, 2.0f, 1.0f), DX::XMVectorSet(0.0f, 0.0f, 5.0f, 1.0f), DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

        auto&& cc = m_Camera.AddComponent<CameraComponent>(ProjectionType::Perspective, (float)m_Window->GetWidth() / (float)m_Window->GetHeight());
		cc.SetPerspectiveHalfAngleFOV(45.0f);
        cc.SetViewMatrix(view);
        cc.SetPerspectiveFar(500.0f);
        cc.SetPerspectiveNear(0.1f);
        cc.SetAspectRatio((float)m_Window->GetWidth() / (float)m_Window->GetHeight());

        m_Light = m_Scene->CreateEntity();
        auto&& dlc = m_Light.AddComponent<DirectionalLightComponent>();
        dlc.LightDirection = { 1.0f,-1.0f,0.0f };
        dlc.SpecularLightColor = { 0.9f,0.9f,0.9f,1.0f };
        dlc.DiffuseLightColor = { 0.4f,0.4f,0.4f,1.0f };
        dlc.AmbientLightColor = { 0.08f,0.08f,0.08f,1.0f };
		m_MonkeyEntity = m_Scene->CreateEntity();
        m_MonkeyEntity.AddComponent<TransformComponent>().Translation = { 0.0f,0.0f,+5.0f };
        DX::XMVECTOR rot = DX::XMQuaternionRotationRollPitchYaw(45.0f,0.0f,0.0f);
        DX::XMStoreFloat4(&m_MonkeyEntity.GetComponent<TransformComponent>().Rotation, rot);
        DX::XMStoreFloat4(&m_Rotation, rot);
        m_MonkeyEntity.GetComponent<TransformComponent>().Scale = { 1.0f,1.0f,1.0f };
        
        m_MonkeyMaterial = GetRenderer()->GetGraphicsDevice()->CreateMaterial("Shaders/PBR.vshader", "Shaders/PBR.pshader");
        m_MonkeyMaterial->SetFillMode(FillMode::FillSolid);
        m_MonkeyMaterial->SetCullMode(CullMode::CullBack);
		MeshAsset meshAsset("Assets/high_poly_monkey.fbx", 0);
        meshAsset.Import();
        m_MonkeyEntity.AddComponent<MeshRendererComponent>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), meshAsset, m_MonkeyMaterial);
        EditorUI::Init(m_Window->GetSDLWindow(), GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(),
            GetRenderer()->GetGraphicsDevice()->GetD3D11DeviceContext().Get());
        Texture2DRef tex = std::make_shared<Texture2D>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(),"Assets/red.png");
        m_MonkeyMaterial->PushTexture(tex);

		return Update();
    }

	auto Application::Update() -> int
	{
        while (m_Running)
        {
            while (m_Window->PollEvents())
            {
                ImGui_ImplSDL2_ProcessEvent(&m_Window->GetEvent());
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
	                    default: break;
	                    }
	                }
                default: break;
                }
            }
            static uint64_t drawCalls = 0;
            auto counter = SDL_GetPerformanceCounter();
            m_Stats.m_FrameTime = counter - m_Stats.m_LastTickCount;
            m_Stats.m_LastTickCount = counter;
            m_Scene->Update();
            EditorUI::NewFrame(m_Window->GetSDLWindow());


            //DXGI_QUERY_VIDEO_MEMORY_INFO info;
            //GetRenderer()->GetSwapChain()->GetDXGIAdapter()->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &info);

            ImGui::NewFrame();
            ImGui::Begin("Dummy", 0, ImGuiWindowFlags_NoBackground| 
                ImGuiWindowFlags_NoTitleBar|
                ImGuiWindowFlags_NoResize| 
                ImGuiWindowFlags_NoMove|
                ImGuiWindowFlags_NoSavedSettings|
				ImGuiWindowFlags_NoInputs);
            ImGui::Text("FPS:%0.2f", m_Stats.GetFPS());
            ImGui::Text("Frame Time:%0.4fms", m_Stats.GetFrameTimeMillis());
            ImGui::Text("Draw Calls:%d", drawCalls);
            //ImGui::Text("Video Memory Budget:%0.2fMB", (float)info.Budget / 1024.0f / 1024.0f);
            //ImGui::Text("Video Memory Usage:%0.2fMB", (float)info.CurrentUsage / 1024.0f / 1024.0f);

            ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
        	ImGui::End();

            static float color[] = { 0.1f,0.1f,0.1f,1.0f };
            ImGui::Begin("Lighting");
            ImGui::SliderFloat3("Light Direction", (float*)&m_Light.GetComponent<DirectionalLightComponent>().LightDirection, -1.0f, 1.0f);
            
            if(ImGui::SliderFloat3("Object Rotation", (float*)&m_Rotation, -3.1415926, 3.1415926))
        		DX::XMStoreFloat4(&m_MonkeyEntity.GetComponent<TransformComponent>().Rotation, DX::XMQuaternionRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z));

            ImGui::SliderFloat("Intensity", &m_Light.GetComponent<DirectionalLightComponent>().Intensity, 0.0f, 500.0f);
            ImGui::ColorPicker4("Diffuse Light Color", (float*)&m_Light.GetComponent<DirectionalLightComponent>().DiffuseLightColor);
            ImGui::ColorPicker4("Ambient Light Color", (float*)&m_Light.GetComponent<DirectionalLightComponent>().AmbientLightColor);
            ImGui::ColorPicker4("Specular Light Color", (float*)&m_Light.GetComponent<DirectionalLightComponent>().SpecularLightColor);
            ImGui::End();

            ImGui::Begin("Renderer");
            ImGui::ColorPicker4("Clear Color", color);
            ImGui::End();

            ImGui::Begin("Material");

            static float roughness = 0.5f;
            static float metallic = 0.1f;
            static float ao = 0.05f;

            ImGui::SliderFloat("Roughness", &roughness, 0.0f, 1.0f);
            ImGui::SliderFloat("Metallic", &metallic, 0.0f, 1.0f);
            ImGui::SliderFloat("AO", &ao, 0.0f, 1.0f);
            
            m_MonkeyMaterial->SetRoughness(roughness);
            m_MonkeyMaterial->SetMetallic(metallic);
            m_MonkeyMaterial->SetAO(ao);

            ImGui::InputText("Diffuse Texture", buf, 512);
           
            if(ImGui::Button("Update Texture"))
            {
                m_MonkeyMaterial->RemoveTexture(0);
                Texture2DRef tex = std::make_shared<Texture2D>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), buf);
            	m_MonkeyMaterial->PushTexture(tex);
            }

            if (ImGui::Button("Destroy Textures"))
            {
                m_MonkeyMaterial->RemoveTexture(0);
            }
            static bool fillSolid = true;
            ImGui::Checkbox("Fill Solid", &fillSolid);
            m_MonkeyMaterial->SetFillMode(fillSolid ? FillMode::FillSolid : FillMode::FillWireframe);

            ImGui::End();

            EditorUI::Render();
            GetRenderer()->Clear(color);
            m_Scene->Draw();
            drawCalls = GetRenderer()->GetDrawCallCount();
            EditorUI::RenderDrawData();
            GetRenderer()->Present(1, 0);
        }


        ImGui_ImplDX11_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        return 0;
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
