#include <Renderer/Commands/CommandManager.h>
#include <Renderer/Enums.h>

namespace Yavr
{
	void CommandManager::Init() noexcept
	{
		m_cmd_pool.Init();
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_cmd_buffers[i].Init(CommandBufferType::LongTime, this);
		m_compute_cmd_buffer.Init(CommandBufferType::LongTime, this);
	}

	void CommandManager::BeginRecord(int active_image_index)
	{
		m_cmd_buffers[active_image_index].BeginRecord();
	}

	void CommandManager::EndRecord(int active_image_index)
	{
		m_cmd_buffers[active_image_index].EndRecord();
	}

	void CommandManager::Destroy() noexcept
	{
		for(int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
			m_cmd_buffers[i].Destroy();
		m_compute_cmd_buffer.Destroy();
		m_cmd_pool.Destroy();
	}
}
