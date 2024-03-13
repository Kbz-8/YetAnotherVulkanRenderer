#include <Platform/Inputs.h>
#include <Core/EventBus.h>
#include <Renderer/Renderer.h>

namespace Yavr
{
	bool Inputs::IsKeyPressed(const std::uint32_t button) const noexcept
	{
		return m_keys[button] == 2;
	}

	bool Inputs::IsKeyReleased(const std::uint32_t button) const noexcept
	{
		return m_keys[button] == 1;
	}

	bool Inputs::IsMouseButtonPressed(const std::uint8_t button) const noexcept
	{
		return m_mouse[button] == 2;
	}

	bool Inputs::IsMouseButtonReleased(const std::uint8_t button) const noexcept
	{
		return m_mouse[button] == 1;
	}

	bool Inputs::HasMouseWheelScrolledUp() const noexcept
	{
		return m_mouse_wheel == 1;
	}

	bool Inputs::HasMouseWheelScrolledDown() const noexcept
	{
		return m_mouse_wheel == 2;
	}

	void Inputs::Update()
	{
		m_x_rel = 0;
		m_y_rel = 0;
		m_mouse_wheel = 0;

		for(int i = 0; i < m_keys.size(); i++)
		{
			if(m_keys[i] == 1)
				m_keys[i] = 0;
		}
		for(int i = 0; i < m_mouse.size(); i++)
		{
			if(m_mouse[i] == 1)
				m_mouse[i] = 0;
		}

		while(SDL_PollEvent(&m_event))
		{
			if(m_event.type == SDL_MOUSEMOTION) 
			{
				m_x = m_event.motion.x;
				m_y = m_event.motion.y;

				m_x_rel = m_event.motion.xrel;
				m_y_rel = m_event.motion.yrel;
			}

			switch(m_event.type) 
			{
				case SDL_KEYDOWN: m_keys[m_event.key.keysym.scancode] = 2; break;
				case SDL_KEYUP: m_keys[m_event.key.keysym.scancode] = 1; break;
				case SDL_MOUSEBUTTONDOWN: m_mouse[m_event.button.button] = 2; break;
				case SDL_MOUSEBUTTONUP: m_mouse[m_event.button.button] = 1; break;

				case SDL_MOUSEWHEEL:
				{
					if(m_event.wheel.y > 0) // scroll up
						m_mouse_wheel = 1;
					else if(m_event.wheel.y < 0) // scroll down
						m_mouse_wheel = 2;
					break;
				}

				case SDL_WINDOWEVENT:
				{
					switch(m_event.window.event)
					{
						case SDL_WINDOWEVENT_CLOSE: m_has_recieved_close_event = true; break;

						case SDL_WINDOWEVENT_RESIZED:
						case SDL_WINDOWEVENT_MINIMIZED:
						case SDL_WINDOWEVENT_MAXIMIZED:
						case SDL_WINDOWEVENT_RESTORED:
						case SDL_WINDOWEVENT_SIZE_CHANGED:
							EventBus::Send("Renderer", ResizeEvent{});
						break;

						default : break;
					}
					break;
				}

				default: break;
			}
		}
	}
}
