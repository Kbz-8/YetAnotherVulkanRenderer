#ifndef __YAVR_VULKAN_DESCRIPTOR_POOL__
#define __YAVR_VULKAN_DESCRIPTOR_POOL__

#include <PreCompiled.h>

namespace Yavr
{
	class DescriptorPool
	{
		public:
			DescriptorPool() = default;

			void Init(std::size_t n, VkDescriptorPoolSize* size);
			void Destroy() noexcept;

			inline VkDescriptorPool& operator()() noexcept { return m_pool; }
			inline VkDescriptorPool& Get() noexcept { return m_pool; }

			~DescriptorPool() = default;

		private:
			VkDescriptorPool m_pool = VK_NULL_HANDLE;
	};
}

#endif
