#ifndef __YAVR_VULKAN_SEMAPHORE__
#define __YAVR_VULKAN_SEMAPHORE__

#include <PreCompiled.h>

namespace Yavr
{
	class Semaphore
	{
		public:
			Semaphore() = default;

			void Init();
			void Destroy() noexcept;

			inline VkSemaphore& GetImageSemaphore() noexcept { return m_image_available_semaphores; }
			inline VkSemaphore& GetRenderImageSemaphore() noexcept { return m_render_finished_semaphores; }

			~Semaphore() = default;

		private:
			VkSemaphore m_image_available_semaphores = VK_NULL_HANDLE;
			VkSemaphore m_render_finished_semaphores = VK_NULL_HANDLE;
	};
}

#endif
