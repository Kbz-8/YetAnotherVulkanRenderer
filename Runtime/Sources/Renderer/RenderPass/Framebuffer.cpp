#include <Renderer/Core/RenderCore.h>
#include <Renderer/RenderPass/Framebuffer.h>
#include <Renderer/Images/Image.h>
#include <Core/Logs.h>

namespace Yavr
{
	void Framebuffer::Init(FramebufferDesc& desc)
	{
		if(m_framebuffer != VK_NULL_HANDLE)
			return;
		if(!desc.renderpass)
			FatalError("Vulkan : unable to create a new frame buffer, render pass pointer has expired");

		m_desc = desc;
		std::vector<VkImageView> attachments;
		for(RenderPassAttachment& att : desc.attachments)
		{
			if(att.image)
				attachments.push_back(att.image->GetImageView());
		}

		VkFramebufferCreateInfo framebuffer_info{};
		framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebuffer_info.renderPass = desc.renderpass->Get();
		framebuffer_info.attachmentCount = static_cast<std::uint32_t>(attachments.size());
		framebuffer_info.pAttachments = attachments.data();
		framebuffer_info.width = desc.width;
		framebuffer_info.height = desc.height;
		framebuffer_info.layers = 1;

		VkResult res = vkCreateFramebuffer(RenderCore::Get().GetDevice().Get(), &framebuffer_info, nullptr, &m_framebuffer);
		if(res != VK_SUCCESS)	
			FatalError("Vulkan : failed to create a framebuffer, %", VerbaliseVkResult(res));
		Message("Vulkan : created new framebuffer");
	}

	void Framebuffer::Destroy() noexcept
	{
		if(m_framebuffer == VK_NULL_HANDLE)
			return;
		vkDestroyFramebuffer(RenderCore::Get().GetDevice().Get(), m_framebuffer, nullptr);
		m_framebuffer = VK_NULL_HANDLE;
		Message("Vulkan : destroyed a framebuffer");
	}
}
