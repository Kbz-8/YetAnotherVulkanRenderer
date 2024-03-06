#include <Renderer/Commands/SingleTimeCommandManager.h>

namespace Yavr
{
	void SingleTimeCmdManager::Init() noexcept
	{
		m_pool.Init();
		for(int i = 0; i < BASE_POOL_SIZE; i++)
		{
			m_buffers.emplace_back();
			m_buffers.back().Init(CommandBufferType::SingleTime, &m_pool);
		}
	}

	CommandBuffer& SingleTimeCmdManager::GetCmdBuffer() noexcept
	{
		for(CommandBuffer& buf : m_buffers)
		{
			if(buf.IsReadyToBeUsed())
			{
				buf.Reset();
				return buf;
			}
		}
		m_buffers.emplace_back().Init(CommandBufferType::SingleTime, &m_pool);
		return _buffers.back();
	}

	void SingleTimeCmdManager::UpdateSingleTimesCmdBuffersSubmitState() noexcept
	{
		for(CommandBuffer& cmd : m_buffers)
			cmd.UpdateSubmitState();
	}

	void SingleTimeCmdManager::WaitForAllExecutions() noexcept
	{
		for(CommandBuffer& cmd : m_buffers)
			cmd.WaitForExecution();
	}

	void SingleTimeCmdManager::Destroy() noexcept
	{
		std::for_each(m_buffers.begin(), m_buffers.end(), [](CommandBuffer& buf)
		{
			buf.Destroy();
		});
		m_pool.Destroy();
	}
}
