#ifndef __YAVR_VULKAN_PIPELINE__
#define __YAVR_VULKAN_PIPELINE__

#include <PreCompiled.h>
#include <Renderer/Commands/CommandBuffer.h>
#include <Renderer/Core/RenderCore.h>
#include <Core/Logs.h>

namespace Yavr
{
	class Pipeline
	{
		public:
			Pipeline() = default;

			inline virtual bool BindPipeline(CommandBuffer& commandBuffer) noexcept { vkCmdBindPipeline(commandBuffer.Get(), GetPipelineBindPoint(), GetPipeline()); return true; }
			inline virtual void EndPipeline([[maybe_unused]] CommandBuffer& commandBuffer) noexcept {};

			virtual const VkPipeline& GetPipeline() const = 0;
			virtual const VkPipelineLayout& GetPipelineLayout() const = 0;
			virtual const VkPipelineBindPoint& GetPipelineBindPoint() const = 0;

			virtual ~Pipeline() = default;
	};

	inline std::vector<std::uint32_t> LoadSpirvFromFile(const std::filesystem::path& path)
	{
		std::ifstream stream(path, std::ios::binary);
		if(!stream.is_open())
			FatalError("Renderer : unable to open a spirv shader file, %", path);
		std::vector<std::uint32_t> data;
		stream.seekg(0);
		std::uint32_t part = 0;
		while(stream.read(reinterpret_cast<char*>(&part), sizeof(part)))
			data.push_back(part);
		stream.close();
		return data;
	}

	inline VkShaderModule CreateShaderModule(const std::vector<std::uint32_t>& code)
	{
		VkShaderModuleCreateInfo create_info{};
		create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		create_info.codeSize = code.size() * sizeof(std::uint32_t);
		create_info.pCode = code.data();

		VkShaderModule module;
		if(vkCreateShaderModule(RenderCore::Get().GetDevice().Get(), &create_info, nullptr, &module) != VK_SUCCESS)
			FatalError("Vulkan : failed to create shader module");
		Message("Vulkan : created shader module");
		return module;
	}
}

#endif
