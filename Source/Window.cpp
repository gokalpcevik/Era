#include "Window.h"

namespace Era
{
	Window::Window(uint32_t w, uint32_t h, std::string_view title)
	{
        m_window = SDL_CreateWindow(title.data(), 50, 50, w, h, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
        if(!m_window)
        {
            ERA_ERROR("Couldn't create window for some reason.");
            return;
        }
        m_pRenderer = std::make_shared<Renderer>(*this);
    }
    Window::Window(uint32_t x, uint32_t y, uint32_t w, uint32_t h, std::string_view title, uint32_t flags)
    {
        m_window = SDL_CreateWindow(title.data(),x,y,w,h,flags);
        if(!m_window)
        {
            ERA_ERROR("Couldn't create window for some reason.");
            return;
        }
        m_pRenderer = std::make_shared<Renderer>(*this);
    }

	Era::Window::~Window()
	{
		SDL_DestroyWindow(m_window);
	}

	int Window::PollEvents()
	{
		return SDL_PollEvent(&m_event);
	}

	uint32_t Window::GetWidth() const
	{
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(m_window, &w, &h);
		return w;
	}

	uint32_t Window::GetSurfaceWidth() const
	{
		auto* surface = SDL_GetWindowSurface(m_window);
		return surface->w;
	}

	uint32_t Window::GetSurfaceHeight() const
	{
		auto* surface = SDL_GetWindowSurface(m_window);
		return surface->h;
	}

	uint32_t Window::GetHeight() const
	{
		int h = 0;
		int w = 0;
		SDL_GetWindowSize(m_window, &w, &h);
		return h;
	}

	HWND Window::GetWin32WindowHandle() const
	{
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(m_window, &info);
		HWND hwnd = info.info.win.window;
		return hwnd;
	}

	bool Window::IsNullPtr() const
    {
        return m_window == nullptr;
    }
}
