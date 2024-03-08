#ifndef __YAVR_RENDERER_VERTEX_BUFFER__
#define __YAVR_RENDERER_VERTEX_BUFFER__

#include <PreCompiled.h>
#include <Renderer/Enums.h>
#include <Utils/CPUBuffer.h>
#include <Renderer/Buffers/GPUBuffer.h>
#include <Renderer/Commands/CommandBuffer.h>

namespace Yavr
{
	class VertexBuffer : public GPUBuffer
	{
		public:
			inline void Init(std::uint32_t size) { GPUBuffer::Init(BufferType::LowDynamic, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, "vertex buffer", {}); }
			void SetData(CPUBuffer data);
			inline void Bind(CommandBuffer& cmd) const noexcept { vkCmdBindVertexBuffers(cmd.Get(), 0, 1, &m_buffer, &m_offset); }
	};
}

#endif
