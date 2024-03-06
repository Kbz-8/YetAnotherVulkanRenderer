#include <Core/Logs.h>
#include <Renderer/Core/Instance.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	void Instance::Init()
	{
		auto extensions = GetRequiredExtensions();

		VkInstanceCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		create_info.enabledExtensionCount = static_cast<std::uint32_t>(extensions.size());
		create_info.ppEnabledExtensionNames = extensions.data();
		create_info.enabledLayerCount = 0; // will be replaced if validation layers are enabled
		create_info.pNext = nullptr;

		VkDebugUtilsMessengerCreateInfoEXT debug_create_info;
		if constexpr(ENABLE_VALIDATION_LAYERS)
		{
			if(RenderCore::Get().GetLayers().CheckValidationLayerSupport())
			{
				create_info.enabledLayerCount = static_cast<std::uint32_t>(validation_layers.size());
				create_info.ppEnabledLayerNames = validation_layers.data();
				RenderCore::Get().GetLayers().PopulateDebugMessengerCreateInfo(debug_create_info);
				create_info.pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&debug_create_info);
			}
		}

		VkResult res;
		if((res = vkCreateInstance(&create_info, nullptr, &m_instance)) != VK_SUCCESS)
			FatalError("Vulkan : failed to create Vulkan instance, %", VerbaliseVkResult(res));
		Message("Vulkan : created new instance");
	}

	std::vector<const char*> Instance::GetRequiredExtensions()
	{
		std::vector<const char*> extensions;
		extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

		#ifdef VK_USE_PLATFORM_XCB_KHR
			extensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_XLIB_KHR
			extensions.push_back(VK_KHR_XLIB_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_WAYLAND_KHR
			extensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_WIN32_KHR
			extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
		#endif

		#ifdef VK_USE_PLATFORM_METAL_EXT
			extensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
		#endif

		if constexpr(ENABLE_VALIDATION_LAYERS)
		{
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}

	void Instance::Destroy() noexcept
	{
		vkDestroyInstance(m_instance, nullptr);
		m_instance = VK_NULL_HANDLE;
		Message("Vulkan : destroyed an instance");
	}
}
