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
		m_depth->TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, RenderCore::Get().GetSingleTimeCmdBuffer());
	}
}
