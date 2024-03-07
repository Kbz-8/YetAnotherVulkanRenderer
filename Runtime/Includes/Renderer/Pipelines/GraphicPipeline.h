#ifndef __YAVR_VULKAN_GRAPHIC_PIPELINE__
#define __YAVR_VULKAN_GRAPHIC_PIPELINE__

#include <Utils/NonOwningPtr.h>
#include <Renderer/Pipelines/Pipeline.h>

namespace Yavr
{
	class GraphicPipeline : public Pipeline
	{
		public:
			GraphicPipeline() = default;

			void Init(NonOwningPtr<class Renderer> renderer);
			bool BindPipeline(CommandBuffer& commandBuffer) noexcept override;
			void EndPipeline(CommandBuffer& commandBuffer) noexcept override;
			void Destroy() noexcept;

			inline const VkPipeline& getPipeline() const override { return _graphicsPipeline; }
			inline const VkPipelineLayout& getPipelineLayout() const override { return _pipelineLayout; }
			inline const VkPipelineBindPoint& getPipelineBindPoint() const override { return _pipelineBindPoint; }
			inline PipelineDesc& getDescription() noexcept { return _desc; }
			
			~GraphicPipeline() = default;

		private:
			void createFrameBuffers();
			void transitionAttachements();

		private:
			std::vector<std::shared_ptr<FrameBuffer>> _frame_buffers;
			PipelineDesc _desc;
			std::shared_ptr<RenderPass> _render_pass;
			VkPipeline _graphicsPipeline = VK_NULL_HANDLE;
			VkPipelineCache _cache = VK_NULL_HANDLE;
			VkPipelineLayout _pipelineLayout = VK_NULL_HANDLE;
			VkPipelineBindPoint _pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			class RendererComponent* _renderer = nullptr;
	};
}

#endif
