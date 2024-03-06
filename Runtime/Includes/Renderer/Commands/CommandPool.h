#ifndef __YARV_VULKAN_COMMAND_POOL__
#define __YARV_VULKAN_COMMAND_POOL__

#include <PreCompiled.h>

namespace Yavr
{
	class CommandPool
	{
		public:
			CommandPool() = default;

			void Init();
			void Destroy() noexcept;

			inline VkCommandPool& operator()() noexcept { return m_cmd_pool; }
			inline VkCommandPool& Get() noexcept { return m_cmd_pool; }

			~CommandPool() = default;

		private:
			VkCommandPool m_cmd_pool = VK_NULL_HANDLE;
	};
}

#endif
