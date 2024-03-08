#include <Core/Logs.h>
#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Renderer.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>
#include <Renderer/Descriptors/DescriptorPool.h>

namespace Yavr
{
	void DescriptorSet::Init(NonOwningPtr<Renderer> renderer, NonOwningPtr<DescriptorSetLayout> layout, NonOwningPtr<DescriptorPool> pool)
	{
		m_renderer = renderer;
		m_layout = layout;
		m_pool = pool;

		auto device = RenderCore::Get().GetDevice().Get();

		std::array<VkDescriptorSetLayout, MAX_FRAMES_IN_FLIGHT> layouts;
		layouts.fill(layout->Get());

		VkDescriptorSetAllocateInfo alloc_info{};
		alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool = m_pool->Get();
		alloc_info.descriptorSetCount = static_cast<std::uint32_t>(MAX_FRAMES_IN_FLIGHT);
		alloc_info.pSetLayouts = layouts.data();

		if(vkAllocateDescriptorSets(device, &alloc_info, m_desc_set.data()) != VK_SUCCESS)
			FatalError("Vulkan : failed to allocate descriptor set");
		Message("Vulkan : descriptor set created");
	}

	void DescriptorSet::WriteDescriptor(int binding, const UniformBuffer& ubo) noexcept
	{
		auto device = RenderCore::Get().GetDevice().Get();

		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			VkDescriptorBufferInfo buffer_info{};
			buffer_info.buffer = ubo.Get(i);
			buffer_info.offset = ubo.GetOffset(i);
			buffer_info.range = ubo.GetSize(i);

			VkWriteDescriptorSet descriptor_write{};
			descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptor_write.dstSet = m_desc_set[i];
			descriptor_write.dstBinding = binding;
			descriptor_write.dstArrayElement = 0;
			descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptor_write.descriptorCount = 1;
			descriptor_write.pBufferInfo = &buffer_info;

			vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);
		}
    }

	void DescriptorSet::WriteDescriptor(int binding, VkImageView view, VkSampler sampler) noexcept
	{
		auto device = RenderCore::Get().GetDevice().Get();

		VkDescriptorImageInfo image_info{};
		image_info.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		image_info.imageView = view;
		image_info.sampler = sampler;

		VkWriteDescriptorSet descriptor_write{};
		descriptor_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptor_write.dstSet = m_desc_set[m_renderer->GetActiveImageIndex()];
		descriptor_write.dstBinding = binding;
		descriptor_write.dstArrayElement = 0;
		descriptor_write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptor_write.descriptorCount = 1;
		descriptor_write.pImageInfo = &image_info;

		vkUpdateDescriptorSets(device, 1, &descriptor_write, 0, nullptr);
	}

	DescriptorSet DescriptorSet::Duplicate() const
	{
		DescriptorSet set;
		set.Init(m_renderer, m_layout, m_pool);
		return set;
	}

	VkDescriptorSet DescriptorSet::operator()() const noexcept
	{
		return m_desc_set[m_renderer->GetActiveImageIndex()];
	}

	VkDescriptorSet DescriptorSet::Get() const noexcept
	{
		return m_desc_set[m_renderer->GetActiveImageIndex()];
	}

	void DescriptorSet::Destroy() noexcept
	{
		vkFreeDescriptorSets(RenderCore::Get().GetDevice().Get(), m_pool->Get(), MAX_FRAMES_IN_FLIGHT, m_desc_set.data());
		Message("Vulkan : descriptor set destroyed");
	}
}
