#ifndef __YAVR_VULKAN_DESCRIPTOR_SET_LAYOUT__
#define __YAVR_VULKAN_DESCRIPTOR_SET_LAYOUT__

#include <PreCompiled.h>

namespace Yavr
{
	class DescriptorSetLayout
	{
		public:
			DescriptorSetLayout() = default;

			void Init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage);
			void Destroy() noexcept;

			inline VkDescriptorSetLayout& operator()() noexcept { return m_layout; }
			inline VkDescriptorSetLayout& Get() noexcept { return m_layout; }
			inline const std::vector<std::pair<int, VkDescriptorType>>& GetBindings() const noexcept { return m_bindings; }

			~DescriptorSetLayout() = default;

		private:
			std::vector<std::pair<int, VkDescriptorType>> m_bindings;
			VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
	};
}

#endif
