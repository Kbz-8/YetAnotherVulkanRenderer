#include "Utils/NonOwningPtr.h"
#include <PreCompiled.h>
#include <Core/Engine.h>
#include <SDL2/SDL.h>
#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <Renderer/Core/RenderCore.h>

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
		m_vertex_buffer.Init(3, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
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
				m_matrices.model = glm::mat4(1.f);
				m_matrices.view = m_camera.GetViewMat();
				m_matrices.projection = m_camera.GetProjectionMat();

				m_vertex_buffer.Bind(m_renderer.GetActiveCmdBuffer());

				m_graphic_pipeline.SetMatricesData(m_matrices);
				m_graphic_pipeline.BindPipeline(m_renderer.GetActiveCmdBuffer());
				m_graphic_pipeline.EndPipeline(m_renderer.GetActiveCmdBuffer());
				m_renderer.EndFrame();
			}

			if(m_running)
				m_running = !m_inputs.HasRecievedCloseEvent();
		}
	}

	Engine::~Engine()
	{
		m_vertex_buffer.Destroy();
		m_graphic_pipeline.Destroy();
		m_renderer.Destroy();
		m_window.Destroy();
		RenderCore::Get().Destroy();
		SDL_Quit();
		Message("Successfully executed !");
	}
}
