#ifndef __YAVR_RENDERER_UNIFORM_BUFFER__
#define __YAVR_RENDERER_UNIFORM_BUFFER__

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/CPUBuffer.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/Buffers/GPUBuffer.h>

namespace Yavr
{
	class UniformBuffer
	{
		public:
			void Init(NonOwningPtr<class Renderer> renderer, std::uint32_t size);
			void SetData(CPUBuffer data);
			void Destroy() noexcept;

			VkDeviceSize GetSize() const noexcept;
			VkDeviceSize GetOffset() const noexcept;
			VkDeviceMemory GetDeviceMemory() const noexcept;
			VkBuffer operator()() const noexcept;
			VkBuffer Get() const noexcept;

			inline VkDeviceSize GetSize(int i) const noexcept { return m_buffers[i].GetSize(); }
			inline VkDeviceSize GetOffset(int i) const noexcept { return m_buffers[i].GetOffset(); }
			inline VkBuffer operator()(int i) const noexcept { return m_buffers[i].Get(); }
			inline VkBuffer Get(int i) const noexcept { return m_buffers[i].Get(); }

		private:
			std::array<GPUBuffer, MAX_FRAMES_IN_FLIGHT> m_buffers;
			std::array<void*, MAX_FRAMES_IN_FLIGHT> m_maps;
			NonOwningPtr<class Renderer> m_renderer;
	};
}

#endif
