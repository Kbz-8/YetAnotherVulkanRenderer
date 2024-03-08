#include <Renderer/Images/Depth.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	void DepthImage::Init(std::uint32_t width, std::uint32_t height)
	{
		VkFormat format = FindSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
		Image::Init(width, height, format, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, { VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT }, "depth_image");
		Image::CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_DEPTH_BIT);
		Image::TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	}
}
