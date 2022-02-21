cmake_minimum_required(VERSION 3.21)

project("Era")

find_package(SDL2 2.0.14 REQUIRED)
find_package(spdlog REQUIRED)
find_package(directxtk CONFIG REQUIRED)
find_package(directxtex CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)

add_executable("Era"
        Renderer/GraphicsDevice.cpp
        Renderer/DeviceContext.cpp
        Renderer/SwapChain.cpp
        Renderer/VertexBuffer.cpp
        Renderer/IndexBuffer.cpp
        Renderer/Texture2D.cpp
        Renderer/InputLayout.cpp
        Renderer/Shader.cpp
        Renderer/Mesh.cpp
        Renderer/Renderer.cpp
        Renderer/InfoQueue.cpp
        Renderer/ErrorChecker.cpp
        Renderer/dxerr.cpp
        Scene/Scene.cpp
        Scene/Entity.cpp
        Scene/Components.h
        Log.cpp
        Application.cpp
        Window.cpp
        Main.cpp)

set_target_properties("Era"
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        CXX_STANDARD 17)


target_include_directories("Era" PRIVATE
        SDL2::SDL2
        spdlog::spdlog
        Microsoft::DirectXTK
        Microsoft::DirectXTex
        assimp::assimp
        EnTT::EnTT
        )
target_link_libraries("Era" PRIVATE
        SDL2::SDL2
        d3d11
        d3dcompiler
        dxguid
        dxgi
        spdlog::spdlog
        Microsoft::DirectXTK
        Microsoft::DirectXTex
        assimp::assimp
        EnTT::EnTT
)