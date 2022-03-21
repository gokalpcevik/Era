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
        Entity box2;
        float xDir = 0.0f;
        float m_LastTickCount = 0.0f;
        float m_FrameTime = 0.0f;

        MaterialRef mat;
        char buf[512] = "Assets/white.png";
    };
}



