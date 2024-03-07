#ifndef __YAVR_VULKAN_SWAPCHAIN__
#define __YAVR_VULKAN_SWAPCHAIN__

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/NonOwningPtr.h>

namespace Yavr
{
	class Swapchain
	{
		public:
			struct SwapchainSupportDetails
			{
				VkSurfaceCapabilitiesKHR capabilities;
				std::vector<VkSurfaceFormatKHR> formats;
				std::vector<VkPresentModeKHR> present_modes;
			};

		public:
			Swapchain() = default;

			void Init(NonOwningPtr<class Renderer> renderer);
			void Recreate();
			void Destroy() noexcept;

			SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			VkPresentModeKHR ChooseSwapPresentMode([[maybe_unused]] const std::vector<VkPresentModeKHR> &available_present_modes);

			inline VkSwapchainKHR Get() noexcept { return m_swapchain; }
			inline VkSwapchainKHR operator()() noexcept { return m_swapchain; }
			inline std::size_t GetImagesNumber() const noexcept { return m_images.size(); }
			inline Image& GetImage(std::size_t i) noexcept { return m_images[i]; }
			inline SwapchainSupportDetails GetSupport() noexcept { return m_swapchain_support; }
			inline VkExtent2D GetExtent() noexcept { return m_extent; }
			inline VkFormat GetImagesFormat() const noexcept { return m_swapchain_image_format; }

			~Swapchain() = default;

		private:
			SwapchainSupportDetails m_swapchain_support;
			VkSwapchainKHR m_swapchain;
			std::vector<Image> m_images;
			VkFormat m_swapchain_image_format;
			VkExtent2D m_extent;
			NonOwningPtr<class Renderer> m_renderer = nullptr;
	};
}

#endif
