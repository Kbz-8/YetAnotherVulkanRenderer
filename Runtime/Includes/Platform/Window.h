#ifndef __YAVR_PLATFORM_WINDOW__
#define __YAVR_PLATFORM_WINDOW__

#include <PreCompiled.h>

namespace Yavr
{
	class Window
	{
		friend class Engine;

		public:
			Window() = default;

			void Init(const std::string& title, std::uint32_t width, std::uint32_t height);

			void Update() noexcept;

			inline const std::string& GetTitle() const noexcept { return m_title; }
			inline int GetWidth() const noexcept { return m_width; } 
			inline int GetHeight() const noexcept { return m_height; } 
			inline SDL_Window* GetSDLWindow() const noexcept { return m_window; }

			~Window();

		private:
			// Can only be called by the engine
			void Destroy() noexcept;

		private:
			SDL_Window* m_window = nullptr;
			std::string m_title;
			int m_height = 0;
			int m_width = 0;
	};
}

#endif
