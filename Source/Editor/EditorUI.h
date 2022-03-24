#pragma once
#include <SDL_video.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_sdl.h>

namespace Era
{
	class EditorUI
	{
	public:
		EditorUI() = default;

		static void Init(SDL_Window* pWindow,ID3D11Device* pDevice,ID3D11DeviceContext* pContext);
		static void NewFrame(SDL_Window* pWindow);
		static void Render();
		static void RenderDrawData();
		static void StyleColors();
	};

}
