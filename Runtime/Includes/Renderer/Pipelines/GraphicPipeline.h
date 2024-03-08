#ifndef __YAVR_VULKAN_GRAPHIC_PIPELINE__
#define __YAVR_VULKAN_GRAPHIC_PIPELINE__

#include <Utils/NonOwningPtr.h>
#include <Renderer/Pipelines/Pipeline.h>
#include <Renderer/RenderPass/Renderpass.h>
#include <Renderer/RenderPass/Framebuffer.h>
#include <Renderer/Descriptors/DescriptorSetLayout.h>
#include <Renderer/Descriptors/DescriptorPool.h>
#include <Renderer/Descriptors/DescriptorSet.h>
#include <Renderer/Buffers/UniformBuffer.h>
#include <Renderer/Matrices.h>
#include <Renderer/Images/Depth.h>

namespace Yavr
{
	class GraphicPipeline : public Pipeline
	{
		public:
			GraphicPipeline() = default;

			void Init(NonOwningPtr<class Renderer> renderer);
			bool BindPipeline(CommandBuffer& command_buffer) noexcept override;
			void SetMatricesData(const MatricesData& data);
			void EndPipeline(CommandBuffer& command_buffer) noexcept override;
			void Destroy() noexcept;

			inline const VkPipeline& GetPipeline() const override { return m_pipeline; }
			inline const VkPipelineLayout& GetPipelineLayout() const override { return m_pipeline_layout; }
			inline const VkPipelineBindPoint& GetPipelineBindPoint() const override { return m_pipeline_bind_point; }

			~GraphicPipeline() = default;

		private:
			void CreateFramebuffers();
			void TransitionAttachments();

		private:
			std::vector<Framebuffer> m_framebuffers;
			std::vector<DescriptorSetLayout> m_descriptor_sets_layout;
			std::vector<DescriptorSet> m_descriptor_sets;
			UniformBuffer m_ubo;
			RenderPass m_renderpass;
			DepthImage m_depth;
			DescriptorPool m_pool;
			VkPipeline m_pipeline = VK_NULL_HANDLE;
			VkPipelineCache _cache = VK_NULL_HANDLE;
			VkPipelineLayout m_pipeline_layout = VK_NULL_HANDLE;
			VkPipelineBindPoint m_pipeline_bind_point = VK_PIPELINE_BIND_POINT_GRAPHICS;
			NonOwningPtr<class Renderer> m_renderer;
	};
}

#endif
