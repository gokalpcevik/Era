#include "Window.h"
#include "../Renderer/Renderer.h"

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

	auto Window::PollEvents() -> int
	{
		switch (m_event.type)
		{
		case SDL_QUIT:
		{
			return 0;
		}
		case SDL_WINDOWEVENT:
		{
			switch (m_event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			{
				this->NotifyWindowResized(m_window, m_event.window.data1, m_event.window.data2);
				break;
			}
			}
		}
		default: break;
		}
		return SDL_PollEvent(&m_event);
	}

	auto Window::GetWidth() const -> uint32_t
	{
		int w = 0;
		int h = 0;
		SDL_GetWindowSize(m_window, &w, &h);
		return w;
	}

	auto Window::GetSurfaceWidth() const -> uint32_t
	{
		auto* surface = SDL_GetWindowSurface(m_window);
		return surface->w;
	}

	auto Window::GetSurfaceHeight() const -> uint32_t
	{
		auto* surface = SDL_GetWindowSurface(m_window);
		return surface->h;
	}

	auto Window::GetHeight() const -> uint32_t
	{
		int h = 0;
		int w = 0;
		SDL_GetWindowSize(m_window, &w, &h);
		return h;
	}

	auto Window::GetWin32WindowHandle() const -> HWND
	{
		SDL_SysWMinfo info;
		SDL_VERSION(&info.version);
		SDL_GetWindowWMInfo(m_window, &info);
		HWND hwnd = info.info.win.window;
		return hwnd;
	}

	auto Window::IsNullPtr() const -> bool
	{
        return m_window == nullptr;
    }

	void Window::Subscribe(IWindowListener* pListener)
	{
		m_Listeners.push_back(pListener);
	}

	void Window::NotifyWindowResized(SDL_Window* resizedWindow, uint32_t width, uint32_t height) const
	{
		for(auto* pListener : m_Listeners)
		{
			pListener->OnWindowResized(resizedWindow, width, height);
		}
	}
}
