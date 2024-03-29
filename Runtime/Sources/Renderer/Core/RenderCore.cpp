#include <Core/Logs.h>
#include <Renderer/Core/RenderCore.h>

namespace Yavr
{
	std::optional<std::uint32_t> FindMemoryType(std::uint32_t type_filter, VkMemoryPropertyFlags properties, bool error)
	{
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties(RenderCore::Get().GetDevice().GetPhysicalDevice(), &mem_properties);

		for(std::uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
		{
			if((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		if(error)
			FatalError("Vulkan : failed to find suitable memory type");
		return std::nullopt;
	}

	const char* VerbaliseVkResult(VkResult result) noexcept
	{
		switch(result)
		{
			case VK_SUCCESS: return "Success";
			case VK_NOT_READY: return "A fence or query has not yet completed";
			case VK_TIMEOUT: return "A wait operation has not completed in the specified time";
			case VK_EVENT_SET: return "An event is signaled";
			case VK_EVENT_RESET: return "An event is unsignaled";
			case VK_INCOMPLETE: return "A return array was too small for the result";
			case VK_ERROR_OUT_OF_HOST_MEMORY: return "A host memory allocation has failed";
			case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "A device memory allocation has failed";
			case VK_ERROR_INITIALIZATION_FAILED: return "Initialization of an object could not be completed for implementation-specific reasons";
			case VK_ERROR_DEVICE_LOST: return "The logical or physical device has been lost";
			case VK_ERROR_MEMORY_MAP_FAILED: return "Mapping of a memory object has failed";
			case VK_ERROR_LAYER_NOT_PRESENT: return "A requested layer is not present or could not be loaded";
			case VK_ERROR_EXTENSION_NOT_PRESENT: return "A requested extension is not supported";
			case VK_ERROR_FEATURE_NOT_PRESENT: return "A requested feature is not supported";
			case VK_ERROR_INCOMPATIBLE_DRIVER: return "The requested version of Vulkan is not supported by the driver or is otherwise incompatible";
			case VK_ERROR_TOO_MANY_OBJECTS: return "Too many objects of the type have already been created";
			case VK_ERROR_FORMAT_NOT_SUPPORTED: return "A requested format is not supported on this device";
			case VK_ERROR_SURFACE_LOST_KHR: return "A surface is no longer available";
			case VK_SUBOPTIMAL_KHR: return "A swapchain no longer matches the surface properties exactly, but can still be used";
			case VK_ERROR_OUT_OF_DATE_KHR: return "A surface has changed in such a way that it is no longer compatible with the swapchain";
			case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "The display used by a swapchain does not use the same presentable image layout";
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "The requested window is already connected to a VkSurfaceKHR, or to some other non-Vulkan API";
			case VK_ERROR_VALIDATION_FAILED_EXT: return "A validation layer found an error";

			default: return "Unknown Vulkan error";
		}
		return nullptr;
	}

	VkPipelineStageFlags AccessFlagsToPipelineStage(VkAccessFlags access_flags, VkPipelineStageFlags stage_flags)
	{
		VkPipelineStageFlags stages = 0;

		while(access_flags != 0)
		{
			VkAccessFlagBits _access_flag = static_cast<VkAccessFlagBits>(access_flags & (~(access_flags - 1)));
			if(_access_flag == 0 || (_access_flag & (_access_flag - 1)) != 0)
				FatalError("Vulkan : an error has been caught during access flag to pipeline stage operation");
			access_flags &= ~_access_flag;

			switch(_access_flag)
			{
				case VK_ACCESS_INDIRECT_COMMAND_READ_BIT: stages |= VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT; break;
				case VK_ACCESS_INDEX_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
				case VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT: stages |= VK_PIPELINE_STAGE_VERTEX_INPUT_BIT; break;
				case VK_ACCESS_UNIFORM_READ_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_INPUT_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT; break;
				case VK_ACCESS_SHADER_READ_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_SHADER_WRITE_BIT: stages |= stage_flags | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT; break;
				case VK_ACCESS_COLOR_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
				case VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; break;
				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
				case VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT: stages |= VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT; break;
				case VK_ACCESS_TRANSFER_READ_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
				case VK_ACCESS_TRANSFER_WRITE_BIT: stages |= VK_PIPELINE_STAGE_TRANSFER_BIT; break;
				case VK_ACCESS_HOST_READ_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
				case VK_ACCESS_HOST_WRITE_BIT: stages |= VK_PIPELINE_STAGE_HOST_BIT; break;
				case VK_ACCESS_MEMORY_READ_BIT: break;
				case VK_ACCESS_MEMORY_WRITE_BIT: break;

				default: Error("Vulkan : unknown access flag"); break;
			}
		}
		return stages;
	}

	void RenderCore::Init()
	{
		if(m_is_init)
			return;
		m_instance.Init();
		m_layers.Init();
		m_device.Init();
		m_queues.Init();
		m_cmd_manager.Init();
		m_is_init = true;
	}

	void RenderCore::Destroy()
	{
		if(!m_is_init)
			return;
		vkDeviceWaitIdle(m_device.Get());
		m_cmd_manager.Destroy();
		m_device.Destroy();
		m_layers.Destroy();
		m_instance.Destroy();
		m_is_init = false;
	}
}
