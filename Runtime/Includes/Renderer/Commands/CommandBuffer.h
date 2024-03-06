#ifndef __YAVR_VULKAN_COMMAND_BUFFER__
#define __YAVR_VULKAN_COMMAND_BUFFER__

#include <Utils/NonOwningPtr.h>
#include <PreCompiled.h>
#include <Renderer/Enums.h>
#include <Renderer/Core/Fence.h>

namespace Yavr
{
	class GPUBuffer;
	class Image;

	class CommandBuffer
	{
		public:
			CommandBuffer() = default;

			void Init(CommandBufferType type, NonOwningPtr<class CommandManager> manager);
			void Init(CommandBufferType type, NonOwningPtr<class CommandPool> pool);
			void Destroy() noexcept;

			void BeginRecord(VkCommandBufferUsageFlags usage = 0);
			void Submit(NonOwningPtr<class Semaphore> semaphores) noexcept;
			void SubmitIdle(bool should_wait_for_execution = true) noexcept;
			void UpdateSubmitState() noexcept;
			inline void WaitForExecution() noexcept { m_fence.Wait(); UpdateSubmitState(); m_state = CommandBufferState::Ready; }
			inline void Reset() noexcept { vkResetCommandBuffer(m_cmd_buffer, 0); }
			void EndRecord();

			void BindVertexBuffer(GPUBuffer& buffer) noexcept;
			void BindIndexBuffer(GPUBuffer& buffer) noexcept;
			void CopyBuffer(GPUBuffer& dst, GPUBuffer& src) noexcept;
			void CopyBufferToImage(GPUBuffer& buffer, Image& image) noexcept;
			void CopyImagetoBuffer(Image& image, GPUBuffer& buffer) noexcept;
			void TransitionImageLayout(Image& image, VkImageLayout new_layout) noexcept;

			inline bool IsInit() const noexcept { return m_state != CommandBufferState::Uninit; }
			inline bool IsReadyToBeUsed() const noexcept { return m_state == CommandBufferState::Ready; }
			inline bool IsRecording() const noexcept { return m_state == CommandBufferState::Recording; }
			inline bool HasBeenSubmitted() const noexcept { return m_state == CommandBufferState::Submitted; }
			inline CommandBufferState GetCurrentState() const noexcept { return m_state; }

			inline VkCommandBuffer& operator()() noexcept { return m_cmd_buffer; }
			inline VkCommandBuffer& Get() noexcept { return m_cmd_buffer; }
			inline Fence& GetFence() noexcept { return m_fence; }

			~CommandBuffer() = default;

		private:
			void PreTransferBarrier() noexcept;
			void PostTransferBarrier() noexcept;

		private:
			Fence m_fence;
			VkCommandBuffer m_cmd_buffer = VK_NULL_HANDLE;
			NonOwningPtr<class CommandPool> m_pool = nullptr;
			CommandBufferState m_state = CommandBufferState::Uninit;
			CommandBufferType m_type;
	};
}

#endif
