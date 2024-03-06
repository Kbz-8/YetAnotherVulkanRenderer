#include <Core/Logs.h>
#include <Renderer/Core/Device.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	const std::vector<const char*> device_extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	void Device::Init()
	{
		PickPhysicalDevice();

		Queues::QueueFamilyIndices indices = RenderCore::Get().GetQueue().GetFamilies();

		std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
		std::set<std::uint32_t> unique_queue_families = { indices.graphics_family.value(), indices.present_family.value() };

		float queue_priority = 1.0f;
		for(std::uint32_t queue_family : unique_queue_families)
		{
			VkDeviceQueueCreateInfo queue_create_info{};
			queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_create_info.queueFamilyIndex = queue_family;
			queue_create_info.queueCount = 1;
			queue_create_info.pQueuePriorities = &queue_priority;
			queue_create_infos.push_back(queue_create_info);
		}

		VkPhysicalDeviceFeatures device_features{};

		VkDeviceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		create_info.queueCreateInfoCount = static_cast<std::uint32_t>(queue_create_infos.size());
		create_info.pQueueCreateInfos = queue_create_infos.data();
		create_info.pEnabledFeatures = &device_features;
		create_info.enabledExtensionCount = static_cast<std::uint32_t>(device_extensions.size());
		create_info.ppEnabledExtensionNames = device_extensions.data();
		create_info.enabledLayerCount = 0;

		VkResult res;
		if((res = vkCreateDevice(m_physical_device, &create_info, nullptr, &m_device)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create logcal device, %", VerbaliseVkResult(res));
		Message("Vulkan : created new logical device");
	}

	void Device::PickPhysicalDevice()
	{
		std::uint32_t device_count = 0;
		vkEnumeratePhysicalDevices(RenderCore::Get().GetInstance().Get(), &device_count, nullptr);

		if(device_count == 0)
			FatalError("Vulkan : failed to find GPUs with Vulkan support");

		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(RenderCore::Get().GetInstance().Get(), &device_count, devices.data());

		SDL_Window* window = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
		if(!window)
			FatalError("Vulkan : failed to create a window to pick physical device");

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		if(SDL_Vulkan_CreateSurface(window, RenderCore::Get().GetInstance().Get(), &surface) != SDL_TRUE)
			FatalError("Vulkan : failed to create a surface to pick physical device");

		std::multimap<int, VkPhysicalDevice> devices_score;

		for(const auto& device : devices)
		{
			int score = DeviceScore(device, surface);
			devices_score.insert(std::make_pair(score, device));
		}

		if(devices_score.rbegin()->first > 0)
			m_physical_device = devices_score.rbegin()->second;
		else
			FatalError("Vulkan : failed to find a suitable GPU");

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		Message("Vulkan : picked a physical device, %", props.deviceName);
		
		RenderCore::Get().GetQueue().FindQueueFamilies(m_physical_device, surface); // update queue indicies to current physical device
		vkDestroySurfaceKHR(RenderCore::Get().GetInstance().Get(), surface, nullptr);
		SDL_DestroyWindow(window);
	}

	int Device::DeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		Queues::QueueFamilyIndices indices = RenderCore::Get().GetQueue().FindQueueFamilies(device, surface);
		bool extensions_supported = CheckDeviceExtensionSupport(device);

		std::uint32_t format_count = 0;
		if(extensions_supported)
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &format_count, nullptr);

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(device, &props);
		if(!indices.IsComplete() || !extensions_supported || format_count == 0)
			return -1;

		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);

		int score = 0;
		if(props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		if(!features.geometryShader)
			return -1;

		score += props.limits.maxImageDimension2D;
		score += props.limits.maxBoundDescriptorSets;
		return score;
	}

	bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		std::uint32_t extension_count;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, nullptr);

		std::vector<VkExtensionProperties> available_extensions(extension_count);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extension_count, available_extensions.data());

		std::set<std::string> required_extensions(device_extensions.begin(), device_extensions.end());

		for(const auto& extension : available_extensions)
			required_extensions.erase(extension.extensionName);

		return required_extensions.empty();
	}

	void Device::Destroy() noexcept
	{
		vkDestroyDevice(m_device, nullptr);
		m_device = VK_NULL_HANDLE;
		Message("Vulkan : destroyed a logical device");
	}
}
