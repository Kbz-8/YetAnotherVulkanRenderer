#include <Renderer/Core/Surface.h>
#include <Renderer/Core/RenderCore.h>
#include <Core/Logs.h>

namespace Yavr
{
	void Surface::Init(const Window& window)
	{
		if(SDL_Vulkan_CreateSurface(window.GetSDLWindow(), RenderCore::Get().GetInstance().Get(), &m_surface) != SDL_TRUE)
			FatalError("Vulkan : failed to create a surface : %", SDL_GetError());
		Message("Vulkan : created new surface");
	}

	VkSurfaceFormatKHR Surface::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& available_formats)
	{
		auto it = std::find_if(available_formats.begin(), available_formats.end(), [](VkSurfaceFormatKHR format)
		{
			return format.format == VK_FORMAT_R8G8B8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
		});

		return (it == available_formats.end() ? available_formats[0] : *it);
	}

	void Surface::Destroy() noexcept
	{
		vkDestroySurfaceKHR(RenderCore::Get().GetInstance().Get(), m_surface, nullptr);
		m_surface = VK_NULL_HANDLE;	
		Message("Vulkan : destroyed a surface");
	}
}
