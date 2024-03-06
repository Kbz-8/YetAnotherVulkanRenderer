#include <Core/Logs.h>
#include <Renderer/Core/Queues.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	Queues::QueueFamilyIndices Queues::FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
		std::uint32_t queue_family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);

		std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

		m_families = Queues::QueueFamilyIndices{};
		int i = 0;
		for(const auto& queue_family : queue_families)
		{
			if(queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				m_families->graphics_family = i;

			VkBool32 present_support = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_support);

			if(present_support)
				m_families->present_family = i;

			if(m_families->IsComplete())
				return *m_families;
			i++;
		}
		return *m_families;
	}

	void Queues::Init()
	{
		if(!m_families.has_value())
		{
			SDL_Window* window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
			if(!window)
				FatalError("Vulkan : failed to create a window to init queues");

			VkSurfaceKHR surface = VK_NULL_HANDLE;
			if(SDL_Vulkan_CreateSurface(window, RenderCore::Get().GetInstance().Get(), &surface) != SDL_TRUE)
				FatalError("Vulkan : failed to create a surface to init queues");

			FindQueueFamilies(RenderCore::Get().GetDevice().GetPhysicalDevice(), surface);

			vkDestroySurfaceKHR(RenderCore::Get().GetInstance().Get(), surface, nullptr);
			SDL_DestroyWindow(window);
		}
		vkGetDeviceQueue(RenderCore::Get().GetDevice().Get(), m_families->graphics_family.value(), 0, &m_graphics_queue);
		vkGetDeviceQueue(RenderCore::Get().GetDevice().Get(), m_families->present_family.value(), 0, &m_present_queue);
		Message("Vulkan : got graphics and present queues");
	}
}
