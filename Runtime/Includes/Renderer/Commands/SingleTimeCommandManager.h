#ifndef __YAVR_VULKAN_SINGLE_TIME_COMMAND_MANAGER__
#define __YAVR_VULKAN_SINGLE_TIME_COMMAND_MANAGER__

#include <PreCompiled.h>
#include <Renderer/Commands/CommandPool.h>

namespace Yavr
{
	class CommandBuffer;

	class SingleTimeCmdManager
	{
		friend class RenderCore;

		public:
			SingleTimeCmdManager() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			void UpdateSingleTimesCmdBuffersSubmitState() noexcept;
			void WaitForAllExecutions() noexcept;

			inline CommandPool& GetCmdPool() noexcept { return m_pool; }
			CommandBuffer& GetCmdBuffer() noexcept;

			~SingleTimeCmdManager() = default;

			inline static constexpr const std::uint8_t BASE_POOL_SIZE = 16;

		private:
			std::vector<CommandBuffer> m_buffers;
			CommandPool m_pool;
	};
}

#endif
