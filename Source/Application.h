//
// Created by Gokalp on 12/15/2021.
//

#pragma once
#include "Window.h"
#include "Log.h"
#include "Scene/Scene.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

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
        auto Update() const -> int;
        auto GetRenderer() const -> const std::shared_ptr<Renderer>& { return m_Window->GetRenderer(); }
    private:
        CommandLineArguments m_CmdArgs{};
        std::shared_ptr<Window> m_Window;
        bool m_Running = true;
        std::shared_ptr<Scene> m_Scene;

        Entity camera;
        Entity box;
        Entity box2;
    };
}



