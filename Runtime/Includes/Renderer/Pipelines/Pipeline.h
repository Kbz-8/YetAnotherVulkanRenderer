#ifndef __YAVR_VULKAN_PIPELINE__
#define __YAVR_VULKAN_PIPELINE__

#include <PreCompiled.h>
#include <Renderer/Commands/CommandBuffer.h>

namespace Yavr
{
	class Pipeline
	{
		public:
			Pipeline() = default;

			inline virtual bool BindPipeline(CommandBuffer& commandBuffer) noexcept { vkCmdBindPipeline(commandBuffer.Get(), GetPipelineBindPoint(), GetPipeline()); return true; }
			inline virtual void EndPipeline(CommandBuffer& commandBuffer) noexcept {};

			virtual const VkPipeline& GetPipeline() const = 0;
			virtual const VkPipelineLayout& GetPipelineLayout() const = 0;
			virtual const VkPipelineBindPoint& GetPipelineBindPoint() const = 0;

			virtual ~Pipeline() = default;
	};
}

#endif
