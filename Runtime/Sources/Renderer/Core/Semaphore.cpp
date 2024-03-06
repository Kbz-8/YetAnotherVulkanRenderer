#include <Core/Logs.h>
#include <Renderer/Core/Semaphore.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	void Semaphore::Init()
	{
		VkSemaphoreCreateInfo semaphore_info{};
		semaphore_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkResult res;
		if(	(res = vkCreateSemaphore(RenderCore::Get().GetDevice().Get(), &semaphore_info, nullptr, &m_image_available_semaphores)) != VK_SUCCESS ||
			(res = vkCreateSemaphore(RenderCore::Get().GetDevice().Get(), &semaphore_info, nullptr, &m_render_finished_semaphores)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a synchronization object (semaphore), %", VerbaliseVkResult(res));
		Message("Vulkan : created new semaphores");
	}

	void Semaphore::Destroy() noexcept
	{
		vkDestroySemaphore(RenderCore::Get().GetDevice().Get(), m_render_finished_semaphores, nullptr);
		m_render_finished_semaphores = VK_NULL_HANDLE;
		vkDestroySemaphore(RenderCore::Get().GetDevice().Get(), m_image_available_semaphores, nullptr);
		m_image_available_semaphores = VK_NULL_HANDLE;
		Message("Vulkan : destroyed semaphores");
	}
}
