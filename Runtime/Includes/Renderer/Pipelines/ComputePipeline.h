#ifndef __YAVR_VULKAN_COMPUTE_PIPELINE__
#define __YAVR_VULKAN_COMPUTE_PIPELINE__

#include <PreCompiled.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>
#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Pipelines/Pipeline.h>

namespace Yavr
{
	class ComputePipeline : public Pipeline
	{
		public:
			ComputePipeline() = default;

			void Init(NonOwningPtr<class Renderer> renderer);
			bool BindPipeline(CommandBuffer& command_buffer) noexcept override;
			void Destroy() noexcept;

			inline const VkPipeline& GetPipeline() const override { return m_pipeline; }
			inline const VkPipelineLayout& GetPipelineLayout() const override { return m_pipeline_layout; }
			inline const VkPipelineBindPoint& GetPipelineBindPoint() const override { return m_pipeline_bind_point; }

			inline DescriptorSet& GetSet(int index) { return m_descriptor_sets[index]; }

			~ComputePipeline() = default;

		private:
			std::vector<DescriptorSetLayout> m_descriptor_sets_layout;
			std::vector<DescriptorSet> m_descriptor_sets;
			DescriptorPool m_pool;
			VkPipeline m_pipeline = VK_NULL_HANDLE;
			VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
			VkPipelineBindPoint m_pipeline_bind_point = VK_PIPELINE_BIND_POINT_COMPUTE;
			NonOwningPtr<class Renderer> m_renderer;
	};
}

#endif
