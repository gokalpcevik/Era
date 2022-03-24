//
// Created by Gokalp on 12/15/2021.
//

#pragma once
#include "Window.h"
#include "Timer.h"
#include "Log.h"
#include <wrl.h>
#include "../Scene/Scene.h"
#include "../Scene/Components.h"
#include "../Scene/Entity.h"
#include "../UserInterface/RenderInterface.h"
#include "../UserInterface/SystemInterface.h"
#include "../UserInterface/SystemInterface.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_sdl.h>
#include "../Editor/EditorUI.h"


namespace Era
{

    struct CommandLineArguments
    {
        size_t Count;
        char** Args;

        auto operator[](size_t Index) const -> char*
        {
            return Args[Index];
        }
    };

    struct ApplicationStats
    {
        Uint64 m_LastTickCount = 0;
        Uint64 m_FrameTime = 0;

        auto GetFPS() const -> float
        {
            return 1.0f / ((float)m_FrameTime / (float)SDL_GetPerformanceFrequency());
        }

        auto GetFrameTimeMillis() const -> float
        {
            return (float)m_FrameTime / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        }

        auto GetFrameTimeSeconds() const -> float
        {
            return (float)m_FrameTime / (float)SDL_GetPerformanceCounter();
        }
    };

    class Application
    {
    public:
        Application() = default;
        static auto Get() -> Application&;
        auto Start() -> int;
        void Exit();
        void SetArgs(CommandLineArguments args) { m_CmdArgs = args; }
    private:
        auto Update() -> int;
        auto GetRenderer() const -> const std::shared_ptr<Renderer>& { return m_Window->GetRenderer(); }
    private:
        CommandLineArguments m_CmdArgs{};
        std::shared_ptr<Window> m_Window;
        Timer m_Timer;
    	bool m_Running = true;
        SceneRef m_Scene;
        Entity camera;
        Entity directionalLight;
        Entity box;
        ApplicationStats m_Stats{};
        MaterialRef mat;
        MaterialRef mat2;
        char buf[512] = "Assets/white.png";
    };
}



