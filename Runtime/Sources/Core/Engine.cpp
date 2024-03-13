#include "Utils/NonOwningPtr.h"
#include <PreCompiled.h>
#include <Core/Engine.h>
#include <SDL2/SDL.h>
#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Vertex.h>

namespace Yavr
{
	namespace Internal
	{
		struct InterruptEvent : public EventBase
		{
			std::uint32_t What() const override { return 168; }
		};
	}

	void FatalErrorEventHandle()
	{
		std::exit(EXIT_FAILURE);
	}

	void SignalHandler([[maybe_unused]] int sig)
	{
		EventBus::Send("Engine", Internal::InterruptEvent{});
	}

	Engine::Engine() : m_camera(0, 0, 0)
	{
		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == 167)
				FatalErrorEventHandle();
			if(event.What() == 168)
				this->Quit();
		};
		EventBus::RegisterListener({ functor, "Engine" });

		signal(SIGINT, SignalHandler);

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
			FatalError("SDL error : unable to init all subsystems : %", SDL_GetError());

		RenderCore::Get().Init();
		m_window.Init("Pipelines Pioneer", 1280, 720);
		m_renderer.Init(&m_window);
		m_graphic_pipeline.Init(&m_renderer);
		m_compute_pipeline.Init(&m_renderer);

		m_vertex_buffer.Init(3 * 27 * 36 * sizeof(Vertex), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

		CPUBuffer vertices(36 * sizeof(Vertex));

		Vertex vertices_data[] = {
			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, 0.0f, -1.0f, 1.0f) },

			{ glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, 0.0f,  1.0f, 1.0f) },

			{ glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(-1.0f, 0.0f, 0.0f, 1.0f) },

			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f) },

			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f), glm::vec4(0.0f, -1.0f, 0.0f, 1.0f) },

			{ glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
			{ glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
			{ glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f), glm::vec4(0.0f,  1.0f, 0.0f, 1.0f) },
		};

		std::memcpy(vertices.GetData(), vertices_data, vertices.GetSize());
		m_vertex_buffer_cube.Init(vertices.GetSize());
		m_vertex_buffer_cube.SetData(vertices);

		m_matrices.model = glm::mat4(1.0f);
	}

	void Engine::Run()
	{
		while(m_running)
		{
			m_inputs.Update();

			m_window.Update();
			m_camera.OnUpdate(static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight()), m_inputs);

			if(m_renderer.BeginFrame())
			{
				if(m_inputs.HasMouseWheelScrolledDown() || m_inputs.HasMouseWheelScrolledUp())
				{
					vkQueueWaitIdle(RenderCore::Get().GetQueue().GetGraphic()); // waiting for the vertex buffer to be idle
					auto cmd_buffer = m_renderer.GetComputeCmdBuffer();
					cmd_buffer.Reset();
					cmd_buffer.BeginRecord();

					m_compute_pipeline.GetSet(0).WriteDescriptor(0, m_vertex_buffer);

					m_compute_pipeline.BindPipeline(cmd_buffer);
					vkCmdDispatch(cmd_buffer.Get(), m_vertex_buffer.GetSize() / 64, 1, 1);

					cmd_buffer.EndRecord();
					cmd_buffer.Submit(CommandBufferSubmit::Compute, nullptr, nullptr);
					cmd_buffer.WaitForExecution();
				}

				//m_matrices.model = glm::rotate(m_matrices.model, glm::radians(2.0f), glm::vec3(1.0f, 0.3f, 0.5f));;
				m_matrices.normal = glm::transpose(glm::inverse(m_matrices.model));
				m_matrices.view = m_camera.GetViewMat();
				m_matrices.projection = m_camera.GetProjectionMat();

				auto render_cmd_buffer = m_renderer.GetActiveCmdBuffer();

				m_graphic_pipeline.SetMatricesData(m_matrices);
				m_graphic_pipeline.BindPipeline(render_cmd_buffer);

				//m_vertex_buffer_cube.Bind(render_cmd_buffer);
				//vkCmdDraw(render_cmd_buffer.Get(), 36, 1, 0, 0);
				m_vertex_buffer.Bind(render_cmd_buffer);
				vkCmdDraw(render_cmd_buffer.Get(), m_vertex_buffer.GetSize() / sizeof(Vertex), 1, 0, 0);

				m_graphic_pipeline.EndPipeline(render_cmd_buffer);
				m_renderer.EndFrame();
			}

			if(m_running)
				m_running = !m_inputs.HasRecievedCloseEvent();
		}
	}

	Engine::~Engine()
	{
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		m_vertex_buffer_cube.Destroy();
		m_vertex_buffer.Destroy();
		m_compute_pipeline.Destroy();
		m_graphic_pipeline.Destroy();
		m_renderer.Destroy();
		m_window.Destroy();
		RenderCore::Get().Destroy();
		SDL_Quit();
		Message("Successfully executed !");
	}
}
