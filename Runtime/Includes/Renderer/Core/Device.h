#ifndef __YAVR_VULKAN_DEVICE__
#define __YAVR_VULKAN_DEVICE__

#include <PreCompiled.h>

namespace Yavr
{
	class Device
	{
		public:
			void Init();
			void Destroy() noexcept;

			inline VkDevice& operator()() noexcept { return m_device; }
			inline VkDevice& Get() noexcept { return m_device; }

			inline VkPhysicalDevice& GetPhysicalDevice() noexcept { return m_physical_device; }

		private:
			void PickPhysicalDevice();
			bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
			int DeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface);

		private:
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
	};
}

#endif

