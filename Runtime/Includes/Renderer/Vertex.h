#ifndef __YAVR_VERTEX__
#define __YAVR_VERTEX__

#include <PreCompiled.h>

namespace Yavr
{
	struct Vertex
	{
		alignas(16) glm::vec4 position;
		alignas(16) glm::vec4 normal;

		Vertex(glm::vec4 pos, glm::vec4 norm) : position(std::move(pos)), normal(std::move(norm)) {}

		inline static VkVertexInputBindingDescription GetBindingDescription();
		inline static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
	};
}

#include <Renderer/Vertex.inl>

#endif
