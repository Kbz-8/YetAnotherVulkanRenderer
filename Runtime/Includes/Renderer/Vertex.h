#ifndef __YAVR_VERTEX__
#define __YAVR_VERTEX__

#include <PreCompiled.h>

namespace Yavr
{
	struct Vertex
	{
		glm::vec3 position;
		glm::vec4 color;

		Vertex(glm::vec3 _pos, glm::vec4 _color) : position(std::move(_pos)), color(std::move(_color)) {}

		inline static VkVertexInputBindingDescription GetBindingDescription();
		inline static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions();
	};
}

#include <Renderer/Vertex.inl>

#endif
