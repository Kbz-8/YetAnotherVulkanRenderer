#include <Core/Logs.h>
#include <Renderer/Core/Fence.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	void Fence::Init()
	{
		VkFenceCreateInfo fence_info{};
		fence_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fence_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		VkResult res;
		if((res = vkCreateFence(RenderCore::Get().GetDevice().Get(), &fence_info, nullptr, &m_fence)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create a synchronization object (fence), %", VerbaliseVkResult(res));
		Message("Vulkan : created new fence");
	}

	void Fence::Wait() noexcept
	{
		vkWaitForFences(RenderCore::Get().GetDevice().Get(), 1, &m_fence, VK_TRUE, UINT64_MAX);
	}

	void Fence::Reset() noexcept
	{
		vkResetFences(RenderCore::Get().GetDevice().Get(), 1, &m_fence);
	}

	bool Fence::IsReady() const noexcept
	{
		return vkGetFenceStatus(RenderCore::Get().GetDevice().Get(), m_fence) == VK_SUCCESS;
	}

	void Fence::Destroy() noexcept
	{
		if(m_fence != VK_NULL_HANDLE)
			vkDestroyFence(RenderCore::Get().GetDevice().Get(), m_fence, nullptr);
		m_fence = VK_NULL_HANDLE;
		Message("Vulkan : destroyed fence");
	}
}
