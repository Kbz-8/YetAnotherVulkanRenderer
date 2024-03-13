#ifndef __YAVR_CORE_ENGINE__
#define __YAVR_CORE_ENGINE__

#include <PreCompiled.h>

#include <Renderer/Renderer.h>
#include <Platform/Window.h>
#include <Platform/Inputs.h>
#include <Core/Camera.h>
#include <Renderer/Buffers/VertexBuffer.h>
#include <Renderer/Pipelines/GraphicPipeline.h>
#include <Renderer/Pipelines/ComputePipeline.h>

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
			ComputePipeline m_compute_pipeline;
			Inputs m_inputs;
			VertexBuffer m_vertex_buffer;
			VertexBuffer m_vertex_buffer_cube;
			Window m_window;
			MatricesData m_matrices;
			bool m_running = true;
	};
}

#endif
