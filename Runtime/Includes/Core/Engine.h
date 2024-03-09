#ifndef __YAVR_CORE_ENGINE__
#define __YAVR_CORE_ENGINE__

#include <PreCompiled.h>

#include <Renderer/Renderer.h>
#include <Platform/Window.h>
#include <Platform/Inputs.h>
#include <Core/Camera.h>
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
			Camera m_camera;
			GraphicPipeline m_graphic_pipeline;
			Inputs m_inputs;
			Window m_window;
			MatricesData m_matrices;
			bool m_running = true;
	};
}

#endif
