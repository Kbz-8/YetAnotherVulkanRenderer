#ifndef __YAVR_SINGLETON__
#define __YAVR_SINGLETON__

#include <Utils/NonCopyable.h>

namespace Yavr
{
	template <typename T>
	class Singleton : public NonCopyable
	{
		public:
			inline static T& Get()
			{
				static T instance;
				return instance;
			}
	};
}

#endif
