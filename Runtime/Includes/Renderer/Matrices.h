#ifndef __YAVR_RENDERER_UNIFORM_DATA__
#define __YAVR_RENDERER_UNIFORM_DATA__

#include <PreCompiled.h>

namespace Yavr
{
	struct MatricesData
	{
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 normal;
		alignas(16) glm::mat4 projection;
	};
}

#endif
