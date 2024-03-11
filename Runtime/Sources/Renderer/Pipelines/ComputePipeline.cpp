#include <Renderer/Pipelines/ComputePipeline.h>
#include <Renderer/Renderer.h>

namespace Yavr
{
	void ComputePipeline::Init(NonOwningPtr<Renderer> renderer)
	{
		m_renderer = renderer;

		auto shader_code = LoadSpirvFromFile(FRACTAL_COMPUTE_SHADER_FILE);
		VkShaderModule shader_module = CreateShaderModule(shader_code);

		VkPipelineShaderStageCreateInfo shader_stage_info{};
		shader_stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shader_stage_info.stage = VK_SHADER_STAGE_COMPUTE_BIT;
		shader_stage_info.module = shader_module;
		shader_stage_info.pName = "main";
	}
}
