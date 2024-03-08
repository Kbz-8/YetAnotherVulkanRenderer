#ifndef __YAVR_VERTEX__
#define __YAVR_VERTEX__

#include <PreCompiled.h>

namespace Yavr
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;

		inline static VkVertexInputBindingDescription GetBindingDescription();
		inline static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
	};
}

#include <Renderer/Vertex.inl>

#endif
