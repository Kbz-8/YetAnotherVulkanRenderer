#include <Renderer/Pipelines/ComputePipeline.h>
#include <Renderer/Renderer.h>

namespace Yavr
{
	void ComputePipeline::Init(NonOwningPtr<Renderer> renderer)
	{
		m_renderer = renderer;

		auto device = RenderCore::Get().GetDevice().Get();

		auto shader_code = LoadSpirvFromFile(FRACTAL_COMPUTE_SHADER_FILE);
		VkShaderModule shader_module = CreateShaderModule(shader_code);

		VkPipelineShaderStageCreateInfo shader_stage_info{};
		shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shader_stage_info.module = shader_module;
		shader_stage_info.pName = "main";

		m_descriptor_sets_layout.emplace_back().Init({ std::make_pair(0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER) }, VK_SHADER_STAGE_COMPUTE_BIT);

		std::vector<VkDescriptorSetLayout> vk_descriptor_layouts;
		for(auto& layout : m_descriptor_sets_layout)
			vk_descriptor_layouts.emplace_back(layout.Get());

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = vk_descriptor_layouts.size();
		pipeline_layout_info.pSetLayouts = vk_descriptor_layouts.data();

		if(vkCreatePipelineLayout(device, &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
			FatalError("Vulkan : faailed to create compute pipeline layout");

		VkComputePipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipeline_info.layout = m_pipeline_layout;
		pipeline_info.stage = shader_stage_info;

		if(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline) != VK_SUCCESS)
			FatalError("Vulkan : failed to create compute pipeline");
		Message("Vulkan : created compute pipeline");

		vkDestroyShaderModule(device, shader_module, nullptr);

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 512 }
		};
		m_pool.Init((sizeof(pool_sizes) / sizeof(VkDescriptorPoolSize)), pool_sizes);
		m_descriptor_sets.emplace_back().Init(m_renderer, &m_descriptor_sets_layout[0], &m_pool);
	}

	bool ComputePipeline::BindPipeline(CommandBuffer& command_buffer) noexcept
	{
		static std::vector<VkDescriptorSet> sets; // cache
		if(sets.empty())
		{
			for(auto& set : m_descriptor_sets)
				sets.emplace_back(set.Get());
		}
		vkCmdBindPipeline(command_buffer.Get(), GetPipelineBindPoint(), m_pipeline);
		vkCmdBindDescriptorSets(command_buffer.Get(), GetPipelineBindPoint(), GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		return true;
	}

	void ComputePipeline::Destroy() noexcept
	{
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		for(auto& layout : m_descriptor_sets_layout)
			layout.Destroy();
		vkDestroyPipeline(RenderCore::Get().GetDevice().Get(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(RenderCore::Get().GetDevice().Get(), m_pipeline_layout, nullptr);
		Message("Vulkan : compute pipeline destroyed");
		m_pool.Destroy();
	}
}
