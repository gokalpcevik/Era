//
// Created by Gokalp on 12/15/2021.
//

#pragma once
#include "Window.h"
#include "Log.h"
#include <wrl.h>
#include "../Scene/Scene.h"
#include "../Scene/Components.h"
#include "../Scene/Entity.h"
#include "../UserInterface/RenderInterface.h"

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
        void CreateMeshes();
    private:
        CommandLineArguments m_CmdArgs{};
        std::shared_ptr<Window> m_Window;
        bool m_Running = true;
        SceneRef m_Scene;

        std::mutex m_CreateMeshesMutex;
        std::future<void> m_CreateMeshesFuture;
        Entity camera;
        Entity directionalLight;
        Entity box;
        Entity box2;
        float xDir = 0.0f;
    };
}



