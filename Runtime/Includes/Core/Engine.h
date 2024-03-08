#ifndef __YAVR_CORE_ENGINE__
#define __YAVR_CORE_ENGINE__

#include <PreCompiled.h>

#include <Renderer/Renderer.h>
#include <Platform/Window.h>
#include <Platform/Inputs.h>
#include <Renderer/Pipelines/GraphicPipeline.h>

namespace Yavr
{
	class Engine
	{
		public:
			Engine();

			void Run();

			inline const Window& GetWindow() const noexcept { return m_window; }
			constexpr void Quit() noexcept { m_running = false; }

			~Engine();

		private:
			Renderer m_renderer;
			GraphicPipeline m_graphic_pipeline;
			Inputs m_inputs;
			Window m_window;
			bool m_running = true;
	};
}

#endif
