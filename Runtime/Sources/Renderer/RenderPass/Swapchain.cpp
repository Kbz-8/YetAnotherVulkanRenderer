#include <Renderer/RenderPass/Swapchain.h>
#include <Renderer/Images/Image.h>
#include <Renderer/Renderer.h>
#include <Core/EventBus.h>
#include <Core/Logs.h>

namespace Yavr
{
	namespace Internal
	{
		struct ResizeEventBroadcast : public EventBase
		{
			std::uint32_t What() const override { return 56; }
		};
	}

	void Swapchain::Init(NonOwningPtr<Renderer> renderer)
	{
		auto device = RenderCore::Get().GetDevice().Get();

		m_renderer = renderer;
		m_swapchain_support = QuerySwapchainSupport(RenderCore::Get().GetDevice().GetPhysicalDevice());

		VkSurfaceFormatKHR surface_format = renderer->GetSurface().ChooseSwapSurfaceFormat(m_swapchain_support.formats);
		VkPresentModeKHR present_mode = ChooseSwapPresentMode(m_swapchain_support.present_modes);
		m_extent = ChooseSwapExtent(m_swapchain_support.capabilities);

		std::uint32_t image_count = m_swapchain_support.capabilities.minImageCount + 1;
		if(m_swapchain_support.capabilities.maxImageCount > 0 && image_count > m_swapchain_support.capabilities.maxImageCount)
			image_count = m_swapchain_support.capabilities.maxImageCount;

		Queues::QueueFamilyIndices indices = RenderCore::Get().GetQueue().FindQueueFamilies(RenderCore::Get().GetDevice().GetPhysicalDevice(), renderer->GetSurface().Get());
		std::uint32_t queue_family_indices[] = { indices.graphics_family.value(), indices.present_family.value() };

		VkSwapchainCreateInfoKHR create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		create_info.surface = renderer->GetSurface().Get();
		create_info.minImageCount = image_count;
		create_info.imageFormat = surface_format.format;
		create_info.imageColorSpace = surface_format.colorSpace;
		create_info.imageExtent = m_extent;
		create_info.imageArrayLayers = 1;
		create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		create_info.preTransform = m_swapchain_support.capabilities.currentTransform;
		create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		create_info.presentMode = present_mode;
		create_info.clipped = VK_TRUE;
		create_info.oldSwapchain = VK_NULL_HANDLE;
		if(indices.graphics_family != indices.present_family)
		{
			create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			create_info.queueFamilyIndexCount = 2;
			create_info.pQueueFamilyIndices = queue_family_indices;
		}
		else
			create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkResult res = vkCreateSwapchainKHR(device, &create_info, nullptr, &m_swapchain);
		if(res != VK_SUCCESS)
			FatalError("Vulkan : failed to create the swapchain, %", VerbaliseVkResult(res));

		std::vector<VkImage> tmp;
		vkGetSwapchainImagesKHR(device, m_swapchain, &image_count, nullptr);
		m_images.resize(image_count);
		tmp.resize(image_count);
		vkGetSwapchainImagesKHR(device, m_swapchain, &image_count, tmp.data());

		for(std::size_t i = 0; i < image_count; i++)
		{
			m_images[i].Init(tmp[i], surface_format.format, m_extent.width, m_extent.height);
			m_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			m_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		m_swapchain_image_format = surface_format.format;
		Message("Vulkan : created new swapchain");
	}

	Swapchain::SwapchainSupportDetails Swapchain::QuerySwapchainSupport(VkPhysicalDevice device)
	{
		Swapchain::SwapchainSupportDetails details;
		VkSurfaceKHR surface = m_renderer->GetSurface().Get();

		if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities) != VK_SUCCESS)
			FatalError("Vulkan : unable to retrieve surface capabilities");

		std::uint32_t format_count = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

		if(format_count != 0)
		{
			details.formats.resize(format_count);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, details.formats.data());
		}

		std::uint32_t present_mode_count;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, nullptr);

		if(present_mode_count != 0)
		{
			details.present_modes.resize(present_mode_count);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &present_mode_count, details.present_modes.data());
		}

		return details;
	}

	VkPresentModeKHR Swapchain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available_present_modes)
	{
		//return VK_PRESENT_MODE_IMMEDIATE_KHR;
		for(const auto& mode : available_present_modes)
		{
			if(mode == VK_PRESENT_MODE_MAILBOX_KHR)
				return mode;
		}
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D Swapchain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if(capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max())
			return capabilities.currentExtent;

		int width, height;
		SDL_Vulkan_GetDrawableSize(m_renderer->GetWindow()->GetSDLWindow(), &width, &height);

		VkExtent2D actual_extent = { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };

		actual_extent.width = std::clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actual_extent.height = std::clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actual_extent;
	}

	void Swapchain::Recreate()
	{
		Destroy();
		Init(m_renderer);
		EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
	}

	void Swapchain::Destroy() noexcept
	{
		if(m_swapchain == VK_NULL_HANDLE)
			return;
		vkDeviceWaitIdle(RenderCore::Get().GetDevice().Get());
		for(Image& img : m_images)
			img.DestroyImageView();
		vkDestroySwapchainKHR(RenderCore::Get().GetDevice().Get(), m_swapchain, nullptr);
		m_swapchain = VK_NULL_HANDLE;
	}
}
