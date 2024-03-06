#include <Core/Logs.h>
#include <Renderer/Buffers/GPUBuffer.h>

namespace Yavr
{
	void GPUBuffer::Init(BufferType type, VkDeviceSize size, VkBufferUsageFlags usage, const char* name, CPUBuffer data)
	{
		if(type == BufferType::Constant)
		{
			if(data.Empty())
			{
				Warning("Vulkan : trying to create constant buffer without data (constant buffers cannot be modified after creation)");
				return;
			}
			m_usage = usage | VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			m_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else if(type == BufferType::HighDynamic)
		{
			m_usage = usage;
			m_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}
		else // LowDynamic
		{
			m_usage = usage;
			m_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}

		m_size = size;

		CreateBuffer(m_usage, m_flags, name);

		if(type == BufferType::Constant || !data.Empty())
		{
			void* mapped = nullptr;
			MapMem(&mapped);
				std::memcpy(mapped, data.GetData(), data.GetSize());
			UnMapMem();
			if(type == BufferType::Constant)
				PushToGPU();
		}
	}

	bool GPUBuffer::CopyFrom(const GPUBuffer& buffer) noexcept
	{
		if(!(m_usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
		{
			Error("Vulkan : buffer cannot be the destination of a copy because it does not have the correct usage flag");
			return false;
		}
		if(!(buffer.m_usage & VK_BUFFER_USAGE_TRANSFER_SRC_BIT))
		{
			Error("Vulkan : buffer cannot be the source of a copy because it does not have the correct usage flag");
			return false;
		}

		CommandBuffer& cmd = RenderCore::Get().GetSingleTimeCmdBuffer();
		cmd.BeginRecord();

		cmd.CopyBuffer(*this, const_cast<GPUBuffer&>(buffer));

		cmd.EndRecord();
		cmd.SubmitIdle();

		return true;
	}

	void GPUBuffer::PushToGPU() noexcept
	{
		GPUBuffer new_buffer;
		new_buffer.m_size = m_size;
		new_buffer.m_usage = (this->m_usage & 0xFFFFFFFC) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		new_buffer.m_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		#ifdef YAVR_DEBUG
			std::string new_name = m_name + "_GPU";
			new_buffer.CreateBuffer(new_buffer.m_usage, new_buffer.m_flags, new_name.c_str());
		#else
			new_buffer.CreateBuffer(new_buffer.m_usage, new_buffer.m_flags, nullptr);
		#endif

		if(new_buffer.CopyFrom(*this))
			Swap(new_buffer);
		new_buffer.Destroy();
	}

	void GPUBuffer::Destroy() noexcept
	{
		vkDestroyBuffer(RenderCore::Get().GetDevice().Get(), m_buffer, nullptr);
		vkFreeMemory(RenderCore::Get().GetDevice().Get(), m_memory, nullptr);
		m_buffer = VK_NULL_HANDLE;
		m_memory = VK_NULL_HANDLE;
	}

	void GPUBuffer::Swap(GPUBuffer& buffer) noexcept
	{
		std::swap(m_buffer, buffer.m_buffer);
		std::swap(m_memory, buffer.m_memory);
		std::swap(m_size, buffer.m_size);
		std::swap(m_offset, buffer.m_offset);
		#ifdef YAVR_DEBUG
			std::swap(m_name, buffer.m_name);
		#endif
		std::swap(m_usage, buffer.m_usage);
		std::swap(m_flags, buffer.m_flags);
		std::swap(m_is_mapped, buffer.m_is_mapped);
	}

	void GPUBuffer::Flush(VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		VkMappedMemoryRange mapped_range{};
		mapped_range.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
		mapped_range.memory = m_memory;
		mapped_range.offset = offset;
		mapped_range.size = size;
		vkFlushMappedMemoryRanges(RenderCore::Get().GetDevice().Get(), 1, &mapped_range);
	}
}
