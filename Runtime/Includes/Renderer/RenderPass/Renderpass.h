#ifndef __YAVR_VULKAN_RENDERPASS__
#define __YAVR_VULKAN_RENDERPASS__

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/Enums.h>

namespace Yavr
{
	struct RenderPassAttachment
	{
		NonOwningPtr<Image> image = nullptr;
		ImageType type = ImageType::Other;

		RenderPassAttachment() = default;
		RenderPassAttachment(NonOwningPtr<Image> i, ImageType ty) : image(i), type(ty) {}
		inline bool operator==(const RenderPassAttachment& rhs) const noexcept { return image == rhs.image && type == rhs.type; }
	};

	class RenderPass
	{
		public:
			RenderPass() = default;

			void Init(std::vector<RenderPassAttachment> attachments);
			void Destroy() noexcept;

			void Begin(class CommandBuffer& cmd, std::array<float, 4> clear, class Framebuffer& fb);
			void End(class CommandBuffer& cmd);

			inline VkRenderPass& operator()() noexcept { return m_renderpass; }
			inline VkRenderPass& Get() noexcept { return m_renderpass; }

			~RenderPass() = default;

		private:
			VkRenderPass m_renderpass = VK_NULL_HANDLE;
			std::vector<VkClearValue> m_clears;
			bool m_depth_only;
			bool m_clear_depth;
			bool m_is_running = false;
	};
}

#endif
