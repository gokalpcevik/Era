//
// Created by Gokalp on 12/15/2021.
//

#include "Application.h"

namespace Era
{
    int Application::Start()
    {
        Log::Init();
        uint32_t w = 1280;
        uint32_t h = 720;
        m_Window = std::make_shared<Window>(150,100,w,h,"ERA",SDL_WINDOW_RESIZABLE);
        if(m_Window->IsNullPtr()) 
            return 0;
        ERA_INFO("Created a window with the following size: {}x{}",w,h);
        m_Scene = std::make_shared<Scene>(GetRenderer());
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
            static float r = 0.0f;
            static float amount = 0.02f;
            r += amount;
            if (r >= 1.0f || r <= 0.0f)
                amount *= -1;
            const float color[] = { 0.0f,0.0f,r,1.0f };
            GetRenderer()->Clear(color);
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
