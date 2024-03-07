#ifndef __YAVR_VULKAN_FRAMEBUFFER__
#define __YAVR_VULKAN_FRAMEBUFFER__

#include <Utils/NonOwningPtr.h>
#include <PreCompiled.h>
#include <Renderer/RenderPass/Renderpass.h>

namespace Yavr
{
	struct FramebufferDesc
	{
		std::vector<RenderPassAttachment> attachments;
		NonOwningPtr<RenderPass> renderpass;
		NonOwningPtr<class RendererComponent> renderer;
		uint32_t width = 0;
		uint32_t height = 0;
		bool screen_fbo = false;
	};

	class Framebuffer
	{
		public:
			Framebuffer() = default;

			void Init(FramebufferDesc& desc);
			void Destroy() noexcept;

			inline VkFramebuffer operator()() noexcept { return m_framebuffer; }
			inline VkFramebuffer Get() noexcept { return m_framebuffer; }

			inline std::uint32_t GetWidth() const noexcept { return m_desc.width; }
			inline std::uint32_t GetHeight() const noexcept { return m_desc.height; }

			~Framebuffer() = default;

		private:
			FramebufferDesc m_desc;
			VkFramebuffer m_framebuffer = VK_NULL_HANDLE;
	};
}

#endif
