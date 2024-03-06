#ifndef __YAVR_VULKAN_INSTANCE__
#define __YAVR_VULKAN_INSTANCE__

#include <PreCompiled.h>

namespace Yavr
{
	class Instance
	{
		public:
			void Init();
			void Destroy() noexcept;

			inline VkInstance& operator()() noexcept { return m_instance; }
			inline VkInstance& Get() noexcept { return m_instance; }

		private:
			std::vector<const char*> GetRequiredExtensions();

		private:
			VkInstance m_instance = VK_NULL_HANDLE;
	};
}

#endif
