#ifndef __YAVR_VULKAN_DESCRIPTOR_SET__
#define __YAVR_VULKAN_DESCRIPTOR_SET__

#include <PreCompiled.h>
#include <Renderer/Core/RenderCore.h>
#include <Utils/NonOwningPtr.h>

namespace Yavr
{
	class DescriptorSet
	{
		public:
			DescriptorSet() = default;

			void Init(NonOwningPtr<class Renderer> renderer, NonOwningPtr<class DescriptorSetLayout> layout, NonOwningPtr<class DescriptorPool> pool);

			void WriteDescriptor(int binding, const class UniformBuffer& ubo) noexcept;
			void WriteDescriptor(int binding, VkImageView view, VkSampler sampler) noexcept;

			inline bool IsInit() noexcept { return m_pool && m_layout && m_renderer; }

			DescriptorSet Duplicate() const;

			void Destroy() noexcept;

			VkDescriptorSet operator()() const noexcept;
			VkDescriptorSet Get() const noexcept;

			~DescriptorSet() = default;

		private:
			std::array<VkDescriptorSet, MAX_FRAMES_IN_FLIGHT> m_desc_set;
			NonOwningPtr<class DescriptorPool> m_pool;
			NonOwningPtr<class DescriptorSetLayout> m_layout;
			NonOwningPtr<class Renderer> m_renderer;
    };
}

#endif
