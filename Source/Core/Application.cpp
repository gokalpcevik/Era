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
        camera = m_Scene->CreateEntity();
        auto view = DX::XMMatrixLookAtLH(DX::XMVectorSet(0.0f, 0.0f, +2.0f, 1.0f), DX::XMVectorSet(0.0f, 0.0f, 5.0f, 1.0f), DX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

        auto&& cc = camera.AddComponent<CameraComponent>(ProjectionType::Perspective, (float)m_Window->GetWidth() / (float)m_Window->GetHeight());
		cc.SetPerspectiveHalfAngleFOV(45.0f);
        cc.SetViewMatrix(view);
        cc.SetPerspectiveFar(500.0f);
        cc.SetPerspectiveNear(0.1f);
        cc.SetAspectRatio((float)m_Window->GetWidth() / (float)m_Window->GetHeight());

        directionalLight = m_Scene->CreateEntity();
        auto&& dlc = directionalLight.AddComponent<DirectionalLightComponent>();
        dlc.LightDirection = { 1.0f,-1.0f,0.0f,1.0f };
        dlc.SpecularLightColor = { 0.9f,0.9f,0.9f,1.0f };
        dlc.DiffuseLightColor = { 0.4f,0.4f,0.4f,1.0f };
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

        mat = GetRenderer()->GetGraphicsDevice()->CreateMaterial("Shaders/Lit.vshader","Shaders/Lit.pshader");
        mat->SetFillMode(FillMode::FillSolid);
        mat->SetCullMode(CullMode::CullBack);
		MeshAsset meshAsset("Assets/monkey.fbx", 0);
        meshAsset.Import();
        box.AddComponent<MeshRendererComponent>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), meshAsset,mat);


        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontFromFileTTF("UIAssets/Fonts/Hack-Regular.ttf", 16);
        // Setup Dear ImGui style
        ImGui::StyleColorsClassic();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForD3D(m_Window->GetSDLWindow());
        ImGui_ImplDX11_Init(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(),
                            GetRenderer()->GetGraphicsDevice()->GetD3D11DeviceContext().Get());

        //Diffuse texture
        Texture2DRef tex = std::make_shared<Texture2D>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(),"Assets/white.png");
        mat->PushTexture(tex);

		return Update();
    }

	auto Application::Update() -> int
	{
        while (m_Running)
        {
            auto ticks = (float)SDL_GetTicks();
            m_FrameTime = ticks - m_LastTickCount;
            m_LastTickCount = ticks;
            m_Scene->Update();
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
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplSDL2_NewFrame(m_Window->GetSDLWindow());
            ImGui::NewFrame();
            ImGui::Begin("Lighting");
            ImGui::ColorPicker4("Diffuse Light Color", (float*)&directionalLight.GetComponent<DirectionalLightComponent>().DiffuseLightColor);
            ImGui::ColorPicker4("Ambient Light Color", (float*)&directionalLight.GetComponent<DirectionalLightComponent>().AmbientLightColor);
            ImGui::ColorPicker4("Specular Light Color", (float*)&directionalLight.GetComponent<DirectionalLightComponent>().SpecularLightColor);
            ImGui::SliderFloat3("Light Direction", (float*)&directionalLight.GetComponent<DirectionalLightComponent>().LightDirection,-1.0f,1.0f);
            ImGui::End();

            ImGui::Begin("Material");
            ImGui::SliderFloat("Shininess", (float*)&directionalLight.GetComponent<DirectionalLightComponent>().Shininess, 1.0f, 500.0f);
            ImGui::InputText("Diffuse Texture", buf, 512);
            if(ImGui::Button("Update Texture"))
            {
                mat->RemoveTexture(0);
                Texture2DRef tex = std::make_shared<Texture2D>(GetRenderer()->GetGraphicsDevice()->GetD3D11Device().Get(), buf);
                mat->PushTexture(tex);
            }

            ImGui::End();
            static float x = 0.01f;
            x += 0.001f * m_FrameTime;
            xDir = std::cos(x);
            //directionalLight.GetComponent<DirectionalLightComponent>().LightDirection = { xDir,-0.75f,0.0f,1.0f };
            constexpr float color[] = { 0.1f,0.1f,0.1f,1.0f };
            ImGui::Render();
            GetRenderer()->Clear(color);
            m_Scene->Draw();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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
