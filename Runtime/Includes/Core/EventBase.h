#ifndef __YAVR_CORE_BASE_EVENT__
#define __YAVR_CORE_BASE_EVENT__

#include <PreCompiled.h>

namespace Yavr
{
	struct EventBase
	{
		virtual std::uint32_t What() const = 0;
	};
}

#endif
