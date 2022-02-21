//
// Created by Gokalp on 12/15/2021.
//

#pragma once
#include "Window.h"
#include "Log.h"
#include "Scene/Scene.h"

namespace Era
{

    struct CommandLineArguments
    {
        size_t Count;
        char** Args;
        char* operator[](size_t Index) const
        {
            return Args[Index];
        }
    };

    class Application
    {
    public:
        Application() = default;
        static Application& Get();
        int Start();
        void Exit();
        void SetArgs(CommandLineArguments args) { m_CmdArgs = args; }
    private:
        int Update() const;
        const std::shared_ptr<Renderer>& GetRenderer() const { return m_Window->GetRenderer(); }
    private:
        CommandLineArguments m_CmdArgs{};
        std::shared_ptr<Window> m_Window;
        bool m_Running = true;
        std::shared_ptr<Scene> m_Scene;
    };
}



