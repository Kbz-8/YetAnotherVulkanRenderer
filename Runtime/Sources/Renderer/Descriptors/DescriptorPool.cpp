#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Core/RenderCore.h>
#include <Core/Logs.h>

namespace Yavr
{
	void DescriptorPool::Init(std::size_t n, VkDescriptorPoolSize* size)
	{
		VkDescriptorPoolCreateInfo pool_info{};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount = n;
		pool_info.pPoolSizes = size;
		pool_info.maxSets = 8192;

		if(vkCreateDescriptorPool(RenderCore::Get().GetDevice().Get(), &pool_info, nullptr, &m_pool) != VK_SUCCESS)
			FatalError("Vulkan : failed to create descriptor pool");
		Message("Vulkan : descriptor pool created");
	}

	void DescriptorPool::Destroy() noexcept
	{
		vkDestroyDescriptorPool(RenderCore::Get().GetDevice().Get(), m_pool, nullptr);
		m_pool = VK_NULL_HANDLE;
		Message("Vulkan : descriptor pool destroyed");
	}
}
