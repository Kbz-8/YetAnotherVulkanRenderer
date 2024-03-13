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
		if((res = vkCreateSemaphore(RenderCore::Get().GetDevice().Get(), &semaphore_info, nullptr, &m_semaphore)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a synchronization object (semaphore), %", VerbaliseVkResult(res));
		Message("Vulkan : created new semaphores");
	}

	void Semaphore::Destroy() noexcept
	{
		vkDestroySemaphore(RenderCore::Get().GetDevice().Get(), m_semaphore, nullptr);
		m_semaphore = VK_NULL_HANDLE;
		Message("Vulkan : destroyed semaphore");
	}
}
