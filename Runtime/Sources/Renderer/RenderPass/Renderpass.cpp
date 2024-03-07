#include <Renderer/RenderPass/Renderpass.h>
#include <Renderer/RenderPass/Framebuffer.h>
#include <Renderer/Commands/CommandBuffer.h>
#include <Renderer/Images/Image.h>
#include <Core/Logs.h>

namespace Yavr
{
	VkAttachmentDescription GetAttachmentDescription(ImageType type, NonOwningPtr<Image> image, bool clear = true)
	{
		VkAttachmentDescription attachment{};

		if(type == ImageType::Color)
		{
			attachment.format = image->GetFormat();
			attachment.initialLayout = image->GetLayout();
			attachment.finalLayout = attachment.initialLayout;
		}
		else if(type == ImageType::Cube)
		{
			Error("Vulkan Render Pass Attachement Description : unsupported (yet) image type '%'", static_cast<int>(type));
			return attachment;
		}
		else if(type == ImageType::Depth)
		{
			attachment.format = image->GetFormat();
			attachment.initialLayout = image->GetLayout();
			attachment.finalLayout = attachment.initialLayout;
		}
		else if(type == ImageType::DepthArray)
		{
			Error("Vulkan Render Pass Attachement Description : unsupported (yet) image type '%'", static_cast<int>(type));
			return attachment;
		}
		else
		{
			Error("Vulkan Render Pass Attachement Description : unsupported image type '%'", static_cast<int>(type));
			return attachment;
		}

		if(clear)
		{
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		}
		else
		{
			attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		}

		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.flags = 0;

		return attachment;
	}

	void RenderPass::Init(std::vector<RenderPassAttachment> attachments)
	{
		std::vector<VkAttachmentDescription> attachments_desc;

		std::vector<VkAttachmentReference> color_attachment_references;
		std::vector<VkAttachmentReference> depth_attachment_references;

		m_depth_only  = true;
		m_clear_depth = false;

		for(uint32_t i = 0; i < attachments.size(); i++)
		{
			attachments_desc.push_back(GetAttachmentDescription(attachments[i].type, attachments[i].image, true));

			if(attachments[i].type == ImageType::Color)
			{
				VkImageLayout layout = attachments[i].image->GetLayout();
				VkAttachmentReference color_attachment_ref{};
				color_attachment_ref.attachment = i;
				color_attachment_ref.layout = layout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL : layout;
				color_attachment_references.push_back(color_attachment_ref);
				m_depth_only = false;
			}
			else if(attachments[i].type == ImageType::Depth)
			{
				VkAttachmentReference depth_attachment_ref{};
				depth_attachment_ref.attachment = i;
				depth_attachment_ref.layout = attachments[i].image->GetLayout();
				depth_attachment_references.push_back(depth_attachment_ref);
				m_clear_depth = true;
			}
		}

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = color_attachment_references.size();
		subpass.pColorAttachments = color_attachment_references.data();
		subpass.pDepthStencilAttachment = depth_attachment_references.data();

		VkRenderPassCreateInfo renderpass_create_info{};
		renderpass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderpass_create_info.attachmentCount = attachments_desc.size();
		renderpass_create_info.pAttachments = attachments_desc.data();
		renderpass_create_info.subpassCount = 1;
		renderpass_create_info.pSubpasses = &subpass;
		renderpass_create_info.dependencyCount = 0;
		renderpass_create_info.pDependencies = nullptr;

		m_clears.resize(attachments.size());

		VkResult res = vkCreateRenderPass(RenderCore::Get().GetDevice().Get(), &renderpass_create_info, nullptr, &m_renderpass);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create render pass, %", VerbaliseVkResult(res));

		Message("Vulkan : created new renderpass");
	}

	void RenderPass::Begin(CommandBuffer& cmd, std::array<float, 4> clear, Framebuffer& fb)
	{
		if(m_is_running)
			return;

		if(!m_depth_only)
		{
			for(int i = 0; i < m_clears.size(); i++)
			{
				m_clears[i].color.float32[0] = clear[0];
				m_clears[i].color.float32[1] = clear[1];
				m_clears[i].color.float32[2] = clear[2];
				m_clears[i].color.float32[3] = clear[3];
			}
		}

		if(m_clear_depth)
			m_clears.back().depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };

		VkRenderPassBeginInfo renderpass_info{};
		renderpass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderpass_info.renderPass = m_renderpass;
		renderpass_info.framebuffer = fb.Get();
		renderpass_info.renderArea.offset = { 0, 0 };
		renderpass_info.renderArea.extent = { fb.GetWidth(), fb.GetHeight() };
		renderpass_info.clearValueCount = static_cast<uint32_t>(m_clears.size());
		renderpass_info.pClearValues = m_clears.data();

		vkCmdBeginRenderPass(cmd.Get(), &renderpass_info, VK_SUBPASS_CONTENTS_INLINE);

		m_is_running = true;
	}

	void RenderPass::End(CommandBuffer& cmd)
	{
		if(!m_is_running)
			return;
		vkCmdEndRenderPass(cmd.Get());
		m_is_running = false;
	}

	void RenderPass::Destroy() noexcept
	{
		vkDestroyRenderPass(RenderCore::Get().GetDevice().Get(), m_renderpass, nullptr);
		m_renderpass = VK_NULL_HANDLE;
		Message("Vulkan : destroyed a renderpass");
	}
}
