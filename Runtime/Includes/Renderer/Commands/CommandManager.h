#ifndef __YAVR_COMMAND_MANAGER__
#define __YAVR_COMMAND_MANAGER__

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Renderer/Commands/CommandBuffer.h>
#include <Renderer/Commands/CommandPool.h>

namespace Yavr
{
	class CommandManager
	{
		public:
			CommandManager() = default;

			void Init() noexcept;
			void BeginRecord(int active_image_index);
			void EndRecord(int active_image_index);
			void Destroy() noexcept;

			inline CommandPool& GetCmdPool() noexcept { return m_cmd_pool; }
			inline CommandBuffer& GetCmdBuffer(int i) noexcept { return m_cmd_buffers[i]; }
			inline CommandBuffer& GetComputeCmdBuffer() noexcept { return m_compute_cmd_buffer; }

			~CommandManager() = default;

		private:
			std::array<CommandBuffer, MAX_FRAMES_IN_FLIGHT> m_cmd_buffers;
			CommandBuffer m_compute_cmd_buffer;
			CommandPool m_cmd_pool;
	};
}

#endif
