#include <Renderer/Buffers/VertexBuffer.h>
#include <Core/Logs.h>

namespace Yavr
{
	void VertexBuffer::SetData(CPUBuffer data)
	{
		if(data.GetSize() > m_size)
		{
			Error("Vulkan : trying to store to much data in a vertex buffer (% bytes in % bytes)", data.GetSize(), m_size);
			return;
		}
		if(data.Empty())
		{
			Warning("Vulkan : cannot set empty data in a vertex buffer");
			return;
		}
		GPUBuffer staging;
		staging.Init(BufferType::Staging, data.GetSize(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, "vertex buffer staging", data);
		CopyFrom(staging);
		staging.Destroy();
	}
}
