#include <Renderer/Descriptors/DescriptorSetLayout.h>
#include <Renderer/Core/RenderCore.h>
#include <Core/Logs.h>

namespace Yavr
{
	void DescriptorSetLayout::Init(std::vector<std::pair<int, VkDescriptorType>> binds, VkShaderStageFlagBits stage)
    {
		std::vector<VkDescriptorSetLayoutBinding> bindings(binds.size());
		for(int i = 0; i < binds.size(); i++)
		{
			bindings[i].binding = binds[i].first;
			bindings[i].descriptorCount = 1;
			bindings[i].descriptorType = binds[i].second;
			bindings[i].pImmutableSamplers = nullptr;
			bindings[i].stageFlags = stage;
		}

		m_bindings = std::move(binds);

		VkDescriptorSetLayoutCreateInfo layout_info{};
		layout_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layout_info.bindingCount = m_bindings.size();
		layout_info.pBindings = bindings.data();

		if(vkCreateDescriptorSetLayout(RenderCore::Get().GetDevice().Get(), &layout_info, nullptr, &m_layout) != VK_SUCCESS)
			FatalError("Vulkan : failed to create descriptor set layout");
	}

	void DescriptorSetLayout::Destroy() noexcept
	{
		vkDestroyDescriptorSetLayout(RenderCore::Get().GetDevice().Get(), m_layout, nullptr);
	}
}
