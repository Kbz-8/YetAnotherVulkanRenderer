#ifndef __YAVR_RENDERER_ENUMS__
#define __YAVR_RENDERER_ENUMS__

#include <PreCompiled.h>

namespace Yavr
{
	enum class BufferType
	{
		Constant = 0,
		HighDynamic, // typically stored in RAM
		LowDynamic,  // typically stored in VRAM

		EndEnum
	};
	constexpr std::size_t BufferTypeCount = static_cast<std::size_t>(BufferType::EndEnum) + 1;

	enum class CommandBufferType
	{
		SingleTime = 0,
		LongTime,

		EndEnum
	};
	constexpr std::size_t CommandBufferTypeCount = static_cast<std::size_t>(CommandBufferType::EndEnum) + 1;

	enum class CommandBufferState
	{
		Uninit = 0,
		Ready,
		Idle,
		Recording,
		Submitted,

		EndEnum
	};
	constexpr std::size_t CommandBufferStateCount = static_cast<std::size_t>(CommandBufferState::EndEnum) + 1;

	enum class ImageType
	{
		Color = 0,
		Depth,
		DepthArray,
		Cube,
		Other,

		EndEnum
	};
	constexpr std::size_t ImageTypeCount = static_cast<std::size_t>(ImageType::EndEnum) + 1;
}

#endif
