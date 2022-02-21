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
		int PollEvents();
		[[nodiscard]] const SDL_Event& GetEvent() const { return m_event; }
		[[nodiscard]] SDL_Window* GetSDLWindow() const { return m_window; }
        [[nodiscard]] bool IsNullPtr() const;
        [[nodiscard]] uint32_t GetWidth() const;
        [[nodiscard]] uint32_t GetSurfaceWidth() const;
        [[nodiscard]] uint32_t GetHeight() const;
        [[nodiscard]] uint32_t GetSurfaceHeight() const;
        [[nodiscard]] HWND GetWin32WindowHandle() const;
        [[nodiscard]] const std::shared_ptr<Renderer>& GetRenderer() const { return m_pRenderer; }
	private:
		SDL_Window* m_window = nullptr;
		SDL_Event m_event{};
		std::shared_ptr<Renderer> m_pRenderer;
	};

}