#ifndef __YAVR_VULKAN_SURFACE__
#define __YAVR_VULKAN_SURFACE__

#include <PreCompiled.h>
#include <Platform/Window.h>

namespace Yavr
{
	class Surface
	{
		public:
			Surface() = default;

			void Init(const Window& window);
			void Destroy() noexcept;
			
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

			inline VkSurfaceKHR& operator()() noexcept { return m_surface; }
			inline VkSurfaceKHR& Get() noexcept { return m_surface; }

			~Surface() = default;

		private:
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	};
}

#endif

