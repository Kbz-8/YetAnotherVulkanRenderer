#ifndef __YAVR_VULKAN_GPU_BUFFER__
#define __YAVR_VULKAN_GPU_BUFFER__

#include <Renderer/Core/RenderCore.h>
#include <PreCompiled.h>
#include <Renderer/Enums.h>
#include <Utils/CPUBuffer.h>

namespace Yavr
{
	class GPUBuffer
	{
		public:
			GPUBuffer() = default;

			void Init(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, CPUBuffer data);
			void Destroy() noexcept;

			inline void MapMem(void** data = nullptr, VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept
			{
				CheckVkResult(vkMapMemory(RenderCore::Get().GetDevice().Get(), m_memory, m_offset + offset, size, 0, data));
				m_is_mapped = true;
			}
			inline bool IsMapped() const noexcept { return m_is_mapped; }
			inline void UnMapMem() noexcept { vkUnmapMemory(RenderCore::Get().GetDevice().Get(), m_memory); m_is_mapped = false; }

			void Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0) noexcept;
			bool CopyFrom(const GPUBuffer& buffer) noexcept;

			void Swap(GPUBuffer& buffer) noexcept;

			inline VkBuffer operator()() const noexcept { return m_buffer; }
			inline VkBuffer Get() const noexcept { return m_buffer; }
			inline VkDeviceMemory GetMemory() const noexcept { return m_memory; }
			inline VkDeviceSize GetSize() const noexcept { return m_size; }
			inline VkDeviceSize GetOffset() const noexcept { return m_offset; }

			~GPUBuffer() = default;

		protected:
			void PushToGPU() noexcept;

		protected:
			VkBuffer m_buffer = VK_NULL_HANDLE;
			VkDeviceMemory m_memory = VK_NULL_HANDLE;
			VkDeviceSize m_offset = 0;
			VkDeviceSize m_size = 0;

		private:
			void CreateBuffer(VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const char* name);

		private:
			#ifdef YAVR_DEBUG
				std::string m_name;
			#endif
			VkBufferUsageFlags m_usage = 0;
			VkMemoryPropertyFlags m_flags = 0;
			bool m_is_mapped = false;
	};
}

#endif
