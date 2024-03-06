#ifndef __YAVR_VULKAN_FENCE__
#define __YAVR_VULKAN_FENCE__

#include <PreCompiled.h>

namespace Yavr
{
	class Fence
	{
		public:
			Fence() = default;

			void Init();
			void Destroy() noexcept;

			inline VkFence& Get() noexcept { return m_fence; }
			void Wait() noexcept;
			void Reset() noexcept;
			bool IsReady() const noexcept;
			inline void WaitAndReset() noexcept { Wait(); Reset(); }

			~Fence() = default;

		private:
			VkFence m_fence = VK_NULL_HANDLE;
	};
}

#endif

