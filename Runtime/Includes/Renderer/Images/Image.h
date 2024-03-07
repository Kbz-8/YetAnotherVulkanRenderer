#ifndef __YAVR_VULKAN_IMAGE__
#define __YAVR_VULKAN_IMAGE__

#include <Utils/NonOwningPtr.h>
#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	std::uint32_t FormatSize(VkFormat format);
	bool IsStencilFormat(VkFormat format);
	bool IsDepthFormat(VkFormat format);
	VkFormat BitsToFormat(std::uint32_t bits);
	VkPipelineStageFlags LayoutToAccessMask(VkImageLayout layout, bool is_destination);

	class Image
	{
		friend class Swapchain;

		public:
			Image() = default;

			inline void Init(VkImage image, VkFormat format, std::uint32_t width, std::uint32_t height, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED) noexcept
			{
				m_image = image;
				m_format = format;
				m_width = width;
				m_height = height;
				m_layout = layout;
			}

			void Init(std::uint32_t width, std::uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, std::vector<VkMemoryPropertyFlags> properties, const char* name);
			void CreateImageView(VkImageViewType type, VkImageAspectFlags aspectFlags) noexcept;
			void CreateSampler() noexcept;
			void CopyFromBuffer(class GPUBuffer& buffer);
			void CopyToBuffer(class GPUBuffer& buffer);
			void TransitionLayout(VkImageLayout new_layout, NonOwningPtr<CommandBuffer> cmd = nullptr);
			virtual void Destroy() noexcept;

			inline VkImage Get() const noexcept { return m_image; }
			inline VkImage operator()() const noexcept { return m_image; }
			inline VkDeviceMemory GetDeviceMemory() const noexcept { return m_memory; }
			inline VkImageView GetImageView() const noexcept { return m_image_view; }
			inline VkFormat GetFormat() const noexcept { return m_format; }
			inline VkImageTiling GetTiling() const noexcept { return m_tiling; }
			inline VkImageLayout GetLayout() const noexcept { return m_layout; }
			inline VkSampler GetSampler() const noexcept { return m_sampler; }
			inline std::uint32_t GetWidth() const noexcept { return m_width; }
			inline std::uint32_t GetHeight() const noexcept { return m_height; }
			inline bool IsInit() const noexcept { return m_image != VK_NULL_HANDLE; }

			virtual ~Image() = default;

		private:
			void DestroySampler() noexcept;
			void DestroyImageView() noexcept;

		private:
			VkDeviceMemory m_memory = VK_NULL_HANDLE;
			VkImage m_image = VK_NULL_HANDLE;
			VkImageView m_image_view = VK_NULL_HANDLE;
			VkSampler m_sampler = VK_NULL_HANDLE;
			#ifdef YAVR_DEBUG
				std::string m_name;
			#endif
			VkFormat m_format;
			VkImageTiling m_tiling;
			VkImageLayout m_layout = VK_IMAGE_LAYOUT_UNDEFINED;
			std::uint32_t m_width = 0;
			std::uint32_t m_height = 0;
	};
}

#endif
