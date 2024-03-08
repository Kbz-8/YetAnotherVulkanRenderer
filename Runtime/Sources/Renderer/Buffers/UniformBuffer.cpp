#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Renderer.h>
#include <Core/Logs.h>

namespace Yavr
{
	void UniformBuffer::Init(NonOwningPtr<Renderer> renderer, std::uint32_t size)
	{
		m_renderer = renderer;
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_buffers[i].Init(BufferType::HighDynamic, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, "uniform buffer", {});
			m_buffers[i].MapMem(&m_maps[i]);
			if(m_maps[i] == nullptr)
				FatalError("Vulkan : unable to map a uniform buffer");
		}
	}

	void UniformBuffer::SetData(CPUBuffer data)
	{
		if(data.GetSize() != m_buffers[m_renderer->GetActiveImageIndex()].GetSize())
		{
			Error("Vulkan : invalid data size to update to a uniform buffer");
			return;
		}
		std::memcpy(m_maps[m_renderer->GetActiveImageIndex()], data.GetData(), data.GetSize());
	}

	VkDeviceSize UniformBuffer::GetSize() const noexcept
	{
		return m_buffers[m_renderer->GetActiveImageIndex()].GetSize();
	}

	VkDeviceSize UniformBuffer::GetOffset() const noexcept
	{
		return m_buffers[m_renderer->GetActiveImageIndex()].GetOffset();
	}

	VkBuffer UniformBuffer::operator()() const noexcept
	{
		return m_buffers[m_renderer->GetActiveImageIndex()].Get();
	}

	VkBuffer UniformBuffer::Get() const noexcept
	{
		return m_buffers[m_renderer->GetActiveImageIndex()].Get();
	}

	void UniformBuffer::Destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_buffers[i].Destroy();
	}
}
