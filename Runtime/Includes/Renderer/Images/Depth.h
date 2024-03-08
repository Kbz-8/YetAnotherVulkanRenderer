#ifndef __YAVR_DEPTH__
#define __YAVR_DEPTH__

#include <PreCompiled.h>
#include <Renderer/Images/Image.h>

namespace Yavr
{
	class DepthImage : public Image
	{
		public:
			DepthImage() = default;
			void Init(std::uint32_t width, std::uint32_t height);
			~DepthImage() = default;
	};
}

#endif
