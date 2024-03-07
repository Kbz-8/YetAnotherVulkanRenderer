#include <Renderer/Renderer.h>
#include <Platform/Window.h>
#include <Core/Logs.h>

namespace Yavr
{
	void Renderer::Init(NonOwningPtr<Window> window)
	{
		m_surface.Init(*window);
		m_swapchain.Init(this);
		m_cmd.Init();

		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_semaphores[i].Init();

		m_framebuffers_resize = false;
	}

	bool Renderer::BeginFrame()
	{
		auto device = RenderCore::Get().GetDevice().Get();

		m_cmd.GetCmdBuffer(m_current_frame_index).WaitForExecution();
		VkResult result = vkAcquireNextImageKHR(device, m_swapchain.Get(), UINT64_MAX, m_semaphores[m_current_frame_index].GetImageSemaphore(), VK_NULL_HANDLE, &m_swapchain_image_index);
		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_swapchain.Recreate();
			return false;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			FatalError("Vulkan error : failed to acquire swapchain image");

		m_cmd.GetCmdBuffer(m_current_frame_index).Reset();
		m_cmd.GetCmdBuffer(m_current_frame_index).BeginRecord();

		return true;
	}

	void Renderer::EndFrame()
	{
		m_cmd.GetCmdBuffer(m_current_frame_index).EndRecord();
		m_cmd.GetCmdBuffer(m_current_frame_index).Submit(&m_semaphores[m_current_frame_index]);

		VkSwapchainKHR swapchain = m_swapchain.Get();
		VkSemaphore signal_semaphores[] = { m_semaphores[m_current_frame_index].GetRenderImageSemaphore() };

		VkPresentInfoKHR present_info{};
		present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount = 1;
		present_info.pWaitSemaphores = signal_semaphores;
		present_info.swapchainCount = 1;
		present_info.pSwapchains = &swapchain;
		present_info.pImageIndices = &m_swapchain_image_index;

		VkResult result = vkQueuePresentKHR(RenderCore::Get().GetQueue().GetPresent(), &present_info);
		if(result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || m_framebuffers_resize)
		{
			m_framebuffers_resize = false;
			m_swapchain.Recreate();
		}
		else if(result != VK_SUCCESS)
			FatalError("Vulkan error : failed to present swap chain image");
		m_current_frame_index = (m_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::Destroy()
	{
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		m_cmd.Destroy();
		m_swapchain.Destroy();
		m_surface.Destroy();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_semaphores[i].Destroy();
	}
}
