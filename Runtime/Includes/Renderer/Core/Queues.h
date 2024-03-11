#ifndef __YAVR_VULKAN_QUEUES__
#define __YAVR_VULKAN_QUEUES__

#include <Core/Logs.h>
#include <PreCompiled.h>

namespace Yavr
{
	class Queues
	{
		public:
			struct QueueFamilyIndices
			{
				std::optional<std::uint32_t> graphics_family;
				std::optional<std::uint32_t> present_family;
				std::optional<std::uint32_t> compute_family;

				inline bool IsComplete() const { return graphics_family.has_value() && compute_family.has_value() && present_family.has_value(); }
			};

		public:
			Queues() = default;

			void Init();

			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

			inline VkQueue& GetGraphic() noexcept { return m_graphics_queue; }
			inline VkQueue& GetPresent() noexcept { return m_present_queue; }
			inline VkQueue& GetCompute() noexcept { return m_compute_queue; }
			inline QueueFamilyIndices GetFamilies() noexcept
			{
				if(m_families.has_value())
					return *m_families;
				FatalError("Vulkan : cannot get queue families, not init");
				return {}; // just to avoid warnings
			}

			~Queues() = default;

		private:
			VkQueue m_graphics_queue;
			VkQueue m_present_queue;
			VkQueue m_compute_queue;
			std::optional<QueueFamilyIndices> m_families;
	};
}

#endif

