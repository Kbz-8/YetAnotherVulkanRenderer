#include <Renderer/Pipelines/GraphicPipeline.h>
#include <Renderer/Renderer.h>
#include <Renderer/Enums.h>
#include <Renderer/Vertex.h>
#include <Core/EventBus.h>

namespace Yavr
{
	std::vector<std::uint32_t> LoadSpirvFromFile(const std::filesystem::path& path)
	{
		std::ifstream stream(path, std::ios::binary);
		if(!stream.is_open())
			FatalError("Renderer : unable to open a spirv shader file, %", path);
		std::vector<std::uint32_t> data;
		stream.seekg(0);
		std::uint32_t part = 0;
		while(stream.read(reinterpret_cast<char*>(&part), sizeof(part)))
			data.push_back(part);
		stream.close();
		return data;
	}

	VkShaderModule CreateShaderModule(const std::vector<std::uint32_t>& code)
	{
		VkShaderModuleCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = code.size() * sizeof(std::uint32_t);
		create_info.pCode = code.data();

		VkShaderModule module;
		if(vkCreateShaderModule(RenderCore::Get().GetDevice().Get(), &create_info, nullptr, &module) != VK_SUCCESS)
			FatalError("Vulkan : failed to create shader module");
		Message("Vulkan : created shader module");
		return module;
	}

	void GraphicPipeline::CreateFramebuffers()
	{
		std::vector<RenderPassAttachment> attachments;
		attachments.emplace_back(&m_renderer->GetSwapchain().GetImage(0), ImageType::Color);
		attachments.emplace_back(&m_depth, ImageType::Depth);

		m_renderpass.Init(attachments);

		FramebufferDesc fbdesc{};
		fbdesc.renderpass = &m_renderpass;
		for(std::size_t i = 0; i < m_renderer->GetSwapchain().GetImagesNumber(); i++)
		{
			attachments[0].image = &m_renderer->GetSwapchain().GetImage(i);
			fbdesc.screen_fbo = true;
			fbdesc.attachments = attachments;
			fbdesc.width = m_renderer->GetSwapchain().GetImage(i).GetWidth();
			fbdesc.height = m_renderer->GetSwapchain().GetImage(i).GetHeight();
			m_framebuffers.emplace_back().Init(fbdesc);
		}
	}

	void GraphicPipeline::TransitionAttachments()
	{
		m_depth.TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}

	void GraphicPipeline::Init(NonOwningPtr<Renderer> renderer)
	{
		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == 56)
			{
				for(auto& fb : m_framebuffers)
					fb.Destroy();
				m_framebuffers.clear();
				m_renderpass.Destroy();
				m_depth.Destroy();
				m_depth.Init(m_renderer->GetSwapchain().GetExtent().width, m_renderer->GetSwapchain().GetExtent().height);
				TransitionAttachments();
				CreateFramebuffers();
			}
		};
		EventBus::RegisterListener({ functor, std::to_string((std::uintptr_t)(void**)this) });

		m_renderer = renderer;
		m_depth.Init(renderer->GetSwapchain().GetExtent().width, renderer->GetSwapchain().GetExtent().height);

		TransitionAttachments();
		CreateFramebuffers();

		auto vert_shader_code = LoadSpirvFromFile(VERTEX_SHADER_FILE);
		auto frag_shader_code = LoadSpirvFromFile(FRAGMENT_SHADER_FILE);
		VkShaderModule vert_shader_module = CreateShaderModule(vert_shader_code);
		VkShaderModule frag_shader_module = CreateShaderModule(frag_shader_code);

		VkPipelineShaderStageCreateInfo vert_shader_stage_info{};
		vert_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vert_shader_stage_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vert_shader_stage_info.module = vert_shader_module;
		vert_shader_stage_info.pName = "main";

		VkPipelineShaderStageCreateInfo frag_shader_stage_info{};
		frag_shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		frag_shader_stage_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		frag_shader_stage_info.module = frag_shader_module;
		frag_shader_stage_info.pName = "main";

		VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_info, frag_shader_stage_info};
		constexpr std::size_t shader_stages_count = sizeof(shader_stages) / sizeof(VkPipelineShaderStageCreateInfo);

		m_descriptor_sets_layout.emplace_back().Init({ std::make_pair(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) }, VK_SHADER_STAGE_VERTEX_BIT);

		VkVertexInputBindingDescription binding_description = Vertex::GetBindingDescription();
		std::array<VkVertexInputAttributeDescription, 2> attributes_description = Vertex::GetAttributeDescriptions();

		VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
		vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertex_input_state_create_info.vertexBindingDescriptionCount = 1;
		vertex_input_state_create_info.pVertexBindingDescriptions = &binding_description;
		vertex_input_state_create_info.vertexAttributeDescriptionCount = attributes_description.size();
		vertex_input_state_create_info.pVertexAttributeDescriptions = attributes_description.data();

		VkPipelineInputAssemblyStateCreateInfo input_assembly{};
		input_assembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		input_assembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		input_assembly.primitiveRestartEnable = VK_FALSE;

		VkDynamicState states[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		constexpr std::size_t states_count = sizeof(states) / sizeof(VkDynamicState);

		VkPipelineDynamicStateCreateInfo dynamic_states{};
		dynamic_states.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamic_states.dynamicStateCount = states_count;
		dynamic_states.pDynamicStates = states;

		VkPipelineViewportStateCreateInfo viewport_state{};
		viewport_state.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewport_state.viewportCount = 1;
		viewport_state.pViewports = nullptr;
		viewport_state.scissorCount = 1;
		viewport_state.pScissors = nullptr;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineDepthStencilStateCreateInfo depth_stencil{};
		depth_stencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depth_stencil.depthTestEnable = VK_TRUE;
		depth_stencil.depthWriteEnable = VK_TRUE;
		depth_stencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depth_stencil.depthBoundsTestEnable = VK_FALSE;
		depth_stencil.stencilTestEnable = VK_FALSE;

		VkPipelineColorBlendAttachmentState color_blend_attachment{};
		color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		color_blend_attachment.blendEnable = VK_FALSE;

		VkPipelineColorBlendStateCreateInfo color_blending{};
		color_blending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		color_blending.logicOpEnable = VK_FALSE;
		color_blending.logicOp = VK_LOGIC_OP_COPY;
		color_blending.attachmentCount = 1;
		color_blending.pAttachments = &color_blend_attachment;
		color_blending.blendConstants[0] = 0.0f;
		color_blending.blendConstants[1] = 0.0f;
		color_blending.blendConstants[2] = 0.0f;
		color_blending.blendConstants[3] = 0.0f;

		std::vector<VkDescriptorSetLayout> vk_descriptor_layouts;
		for(auto& layout : m_descriptor_sets_layout)
			vk_descriptor_layouts.emplace_back(layout.Get());

		VkPipelineLayoutCreateInfo pipeline_layout_info{};
		pipeline_layout_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipeline_layout_info.setLayoutCount = vk_descriptor_layouts.size();
		pipeline_layout_info.pSetLayouts = vk_descriptor_layouts.data();

		if(vkCreatePipelineLayout(RenderCore::Get().GetDevice().Get(), &pipeline_layout_info, nullptr, &m_pipeline_layout) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a graphics pipeline layout");

		VkGraphicsPipelineCreateInfo pipeline_info{};
		pipeline_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipeline_info.stageCount = shader_stages_count;
		pipeline_info.pStages = shader_stages;
		pipeline_info.pVertexInputState = &vertex_input_state_create_info;
		pipeline_info.pInputAssemblyState = &input_assembly;
		pipeline_info.pViewportState = &viewport_state;
		pipeline_info.pRasterizationState = &rasterizer;
		pipeline_info.pMultisampleState = &multisampling;
		pipeline_info.pColorBlendState = &color_blending;
		pipeline_info.pDynamicState = &dynamic_states;
		pipeline_info.layout = m_pipeline_layout;
		pipeline_info.renderPass = m_renderpass.Get();
		pipeline_info.subpass = 0;
		pipeline_info.basePipelineHandle = VK_NULL_HANDLE;
		pipeline_info.pDepthStencilState = &depth_stencil;

		if(vkCreateGraphicsPipelines(RenderCore::Get().GetDevice().Get(), VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a graphics pipeline");
		Message("Vulkan : created new graphic pipeline");

		vkDestroyShaderModule(RenderCore::Get().GetDevice().Get(), frag_shader_module, nullptr);
		vkDestroyShaderModule(RenderCore::Get().GetDevice().Get(), vert_shader_module, nullptr);

		m_ubo.Init(m_renderer, sizeof(MatricesData));

		VkDescriptorPoolSize pool_sizes[] = {
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 512 }
		};
		m_pool.Init((sizeof(pool_sizes) / sizeof(VkDescriptorPoolSize)), pool_sizes);
		m_descriptor_sets.emplace_back().Init(m_renderer, &m_descriptor_sets_layout[0], &m_pool);
		m_descriptor_sets.back().WriteDescriptor(0, m_ubo);
	}

	void GraphicPipeline::SetMatricesData(const MatricesData& data)
	{
		CPUBuffer buffer(sizeof(MatricesData));
		std::memcpy(buffer.GetData(), &data, buffer.GetSize());
		m_ubo.SetData(std::move(buffer));
	}

	bool GraphicPipeline::BindPipeline(CommandBuffer& command_buffer) noexcept
	{
		if(!m_renderer->IsRendering())
			return false;
		auto& fb = m_framebuffers[m_renderer->GetSwapchainImageIndex()];

		static std::vector<VkDescriptorSet> sets; // cache
		if(sets.empty())
		{
			for(auto& set : m_descriptor_sets)
				sets.emplace_back(set.Get());
		}

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = fb.GetWidth();
		viewport.height = fb.GetHeight();
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer.Get(), 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent.width = fb.GetWidth();
		scissor.extent.height = fb.GetHeight();
		vkCmdSetScissor(command_buffer.Get(), 0, 1, &scissor);

		vkCmdBindPipeline(command_buffer.Get(), GetPipelineBindPoint(), GetPipeline());

		m_renderpass.Begin(command_buffer, { 0.f, 0.f, 0.f, 0.f }, fb);

		vkCmdBindDescriptorSets(m_renderer->GetActiveCmdBuffer().Get(), GetPipelineBindPoint(), GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
		return true;
	}

	void GraphicPipeline::EndPipeline(CommandBuffer& command_buffer) noexcept
	{
		if(!m_renderer->IsRendering())
			return;
		m_renderpass.End(command_buffer);
	}

	void GraphicPipeline::Destroy() noexcept
	{
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		for(auto& fb : m_framebuffers)
			fb.Destroy();
		m_depth.Destroy();
		m_renderpass.Destroy();
		for(auto& layout : m_descriptor_sets_layout)
			layout.Destroy();
		vkDestroyPipeline(RenderCore::Get().GetDevice().Get(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(RenderCore::Get().GetDevice().Get(), m_pipeline_layout, nullptr);
		m_ubo.Destroy();
		m_pool.Destroy();
		Message("Vulkan : graphic pipeline destroyed");
	}
}
