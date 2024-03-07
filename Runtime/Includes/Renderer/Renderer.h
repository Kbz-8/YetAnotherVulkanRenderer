#ifndef __YAVR_RENDERER__
#define __YAVR_RENDERER__

#include <PreCompiled.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/Commands/CommandManager.h>
#include <Renderer/RenderPass/Swapchain.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Core/Semaphore.h>
#include <Renderer/Core/Surface.h>
#include <Renderer/Images/Image.h>
#include <Core/EventBase.h>

namespace Yavr
{
	struct ResizeEvent : public EventBase
	{
		std::uint32_t What() const override { return 24; }
	};

	class Renderer
	{
		public:
			Renderer() = default;

			void Init(NonOwningPtr<class Window> window);

			bool BeginFrame();
			void EndFrame();

			void Destroy();

			inline NonOwningPtr<class Window> GetWindow() { return m_window; }
			inline Surface& GetSurface() noexcept { return m_surface; }
			inline CommandPool& GetCmdPool() noexcept { return m_cmd.GetCmdPool(); }
			inline Swapchain& GetSwapchain() noexcept { return m_swapchain; }
			inline Semaphore& GetSemaphore(int i) noexcept { return m_semaphores[i]; }
			inline CommandBuffer& GetCmdBuffer(int i) noexcept { return m_cmd.GetCmdBuffer(i); }
			inline CommandBuffer& GetActiveCmdBuffer() noexcept { return m_cmd.GetCmdBuffer(m_current_frame_index); }
			inline std::uint32_t GetActiveImageIndex() noexcept { return m_current_frame_index; }
			inline std::uint32_t GetImageIndex() noexcept { return m_swapchain_image_index; }

			constexpr inline void RequireFramebufferResize() noexcept { m_framebuffers_resize = true; }

			~Renderer() = default;

		private:
			CommandManager m_cmd;
			Surface m_surface;
			Swapchain m_swapchain;
			std::array<Semaphore, MAX_FRAMES_IN_FLIGHT> m_semaphores;

			NonOwningPtr<class Window> m_window = nullptr;

			std::uint32_t m_current_frame_index = 0;
			std::uint32_t m_swapchain_image_index = 0;
			bool m_framebuffers_resize = false;
	};
}

#endif
