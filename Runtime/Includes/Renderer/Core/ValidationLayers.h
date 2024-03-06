#ifndef __YAVR_VULKAN_VALIDATION_LAYERS__
#define __YAVR_VULKAN_VALIDATION_LAYERS__

#include <PreCompiled.h>

namespace Yavr
{
	class ValidationLayers
	{
		public:
			ValidationLayers() = default;

			void Init();
			void Destroy();

			bool CheckValidationLayerSupport();
			void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& create_info);

			VkResult SetDebugUtilsObjectNameEXT(VkObjectType object_type, std::uint64_t object_handle, const char* object_name);

			~ValidationLayers() = default;

		private:
			VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator);
			static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
			void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks* pAllocator);

		private:
			VkDebugUtilsMessengerEXT m_debug_messenger;
			PFN_vkSetDebugUtilsObjectNameEXT f_vkSetDebugUtilsObjectNameEXT = nullptr;
	};
}

#endif

