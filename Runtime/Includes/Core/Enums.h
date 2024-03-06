#ifndef __YAVR_CORE_ENUMS__
#define __YAVR_CORE_ENUMS__

#include <PreCompiled.h>

namespace Yavr
{
	enum class LogType
	{
		Message = 0,
		Warning,
		Error,
		FatalError,

		EndEnum
	};

	constexpr std::size_t LogTypeCount = static_cast<std::size_t>(LogType::EndEnum) + 1;
}

#endif
