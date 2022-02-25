#pragma once
#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <string_view>
#include "Log.h"
#include "Renderer/Renderer.h"

namespace Era
{
	class Window
	{
	public:
        Window(uint32_t w, uint32_t h,std::string_view title);
        Window(uint32_t x, uint32_t y,uint32_t w, uint32_t h,std::string_view title,uint32_t flags);
		Window(const Window&) = delete;
		virtual ~Window();
        auto PollEvents() -> int;
		[[nodiscard]] auto GetEvent() const -> const SDL_Event& { return m_event; }
		[[nodiscard]] auto GetSDLWindow() const -> SDL_Window* { return m_window; }
        [[nodiscard]] auto IsNullPtr() const -> bool;
        [[nodiscard]] auto GetWidth() const -> uint32_t;
        [[nodiscard]] auto GetSurfaceWidth() const -> uint32_t;
        [[nodiscard]] auto GetHeight() const -> uint32_t;
        [[nodiscard]] auto GetSurfaceHeight() const -> uint32_t;
        [[nodiscard]] auto GetWin32WindowHandle() const -> HWND;
        [[nodiscard]] auto GetRenderer() const -> const std::shared_ptr<Renderer>& { return m_pRenderer; }
	private:
		SDL_Window* m_window = nullptr;
		SDL_Event m_event{};
		std::shared_ptr<Renderer> m_pRenderer;
	};

}