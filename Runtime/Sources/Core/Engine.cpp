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

		struct UBOComputeGenerator
		{
			std::uint32_t iterations;
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

		m_ubo_compute_generator.Init(&m_renderer, sizeof(Internal::UBOComputeGenerator));
		m_compute_pipeline.GetSet(0).WriteDescriptor(1, m_ubo_compute_generator);

		m_matrices.model = glm::mat4(1.0f);
	}

	void Engine::Run()
	{
		static bool first_run = true;

		while(m_running)
		{
			m_inputs.Update();

			m_window.Update();
			m_camera.OnUpdate(static_cast<float>(m_window.GetWidth()) / static_cast<float>(m_window.GetHeight()), m_inputs);

			if(first_run || (m_inputs.IsKeyPressed(SDL_SCANCODE_UP) || m_inputs.IsKeyPressed(SDL_SCANCODE_DOWN) ||
				m_inputs.HasMouseWheelScrolledDown() || m_inputs.HasMouseWheelScrolledUp()))
			{
				for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) // waiting for the vertex buffer to be idle
					m_renderer.GetCmdBuffer(i).WaitForExecution();

				if(m_inputs.IsKeyPressed(SDL_SCANCODE_UP) || m_inputs.HasMouseWheelScrolledUp())
					m_iterations++;
				else if(m_inputs.IsKeyPressed(SDL_SCANCODE_DOWN) || m_inputs.HasMouseWheelScrolledDown())
					m_iterations--;

				if(m_iterations < 1)
					m_iterations = 1;
				else if(m_iterations > 64)
					m_iterations = 64;

				std::cout << "iterations : " << m_iterations << std::endl;

				CPUBuffer buffer(sizeof(Internal::UBOComputeGenerator));
				std::memcpy(buffer.GetData(), &m_iterations, buffer.GetSize());
				m_ubo_compute_generator.SetData(std::move(buffer));

				m_vertex_buffer.Destroy();
				m_vertex_buffer.Init(m_iterations * 20 * 36 * sizeof(Vertex), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
				m_compute_pipeline.GetSet(0).WriteDescriptor(0, m_vertex_buffer);

				auto cmd_buffer = m_renderer.GetComputeCmdBuffer();
				cmd_buffer.Reset();
				cmd_buffer.BeginRecord();

				m_compute_pipeline.BindPipeline(cmd_buffer);
				std::uint32_t group_size = std::cbrt(m_vertex_buffer.GetSize());
				vkCmdDispatch(cmd_buffer.Get(), group_size, group_size, group_size);

				cmd_buffer.EndRecord();
				cmd_buffer.Submit(CommandBufferSubmit::Compute, nullptr, nullptr);
				cmd_buffer.WaitForExecution();
			}

			if(m_renderer.BeginFrame())
			{
				m_matrices.normal = glm::transpose(glm::inverse(m_matrices.model));
				m_matrices.view = m_camera.GetViewMat();
				m_matrices.projection = m_camera.GetProjectionMat();

				auto render_cmd_buffer = m_renderer.GetActiveCmdBuffer();

				m_graphic_pipeline.SetMatricesData(m_matrices);
				m_graphic_pipeline.BindPipeline(render_cmd_buffer);

				if(m_vertex_buffer.IsInit())
				{
					m_vertex_buffer.Bind(render_cmd_buffer);
					vkCmdDraw(render_cmd_buffer.Get(), m_vertex_buffer.GetSize() / sizeof(Vertex), 1, 0, 0);
				}

				m_graphic_pipeline.EndPipeline(render_cmd_buffer);
				m_renderer.EndFrame();
			}

			if(m_running)
				m_running = !m_inputs.HasRecievedCloseEvent();
			first_run = false;
		}
	}

	Engine::~Engine()
	{
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		m_ubo_compute_generator.Destroy();
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
