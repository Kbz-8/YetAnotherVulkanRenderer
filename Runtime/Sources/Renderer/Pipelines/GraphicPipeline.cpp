#include <Renderer/Pipelines/GraphicPipeline.h>
#include <Renderer/Renderer.h>
#include <Renderer/Enums.h>

namespace Yavr
{
	void GraphicPipeline::CreateFramebuffers()
	{
		std::vector<RenderPassAttachment> attachments;
		m_framebuffers.clear();

		attachments.emplace_back(&m_renderer->GetSwapchain().GetImage(0), ImageType::Color);
		attachments.emplace_back(m_depth, ImageType::Depth);

		m_renderpass.Destroy();
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
		m_depth.TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, &RenderCore::Get().GetSingleTimeCmdBuffer());
	}

	void GraphicPipeline::Init(NonOwningPtr<Renderer> renderer)
	{
	}

	bool GraphicPipeline::BindPipeline(CommandBuffer& command_buffer) noexcept
	{
		if(m_renderer->IsFramebufferResizeRequested())
		{
			TransitionAttachments();
			CreateFramebuffers();
		}
		if(!m_renderer->IsRendering())
			return false;
		auto& fb = m_framebuffers[m_renderer->GetSwapchainImageIndex()];

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
		vkDestroyPipeline(RenderCore::Get().GetDevice().Get(), m_pipeline, nullptr);
		vkDestroyPipelineLayout(RenderCore::Get().GetDevice().Get(), m_pipeline_layout, nullptr);
		Message("Vulkan : graphic pipeline destroyed");
	}
}
