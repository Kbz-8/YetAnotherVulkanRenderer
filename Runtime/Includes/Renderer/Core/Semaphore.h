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

			inline VkSemaphore& Get() noexcept { return m_semaphore; }

			~Semaphore() = default;

		private:
			VkSemaphore m_semaphore = VK_NULL_HANDLE;
	};
}

#endif
