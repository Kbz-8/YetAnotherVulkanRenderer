#ifndef __YAVR_UTILS_NON_COPYABLE__
#define __YAVR_UTILS_NON_COPYABLE__

namespace Yavr
{
	class NonCopyable
	{
		protected:
			NonCopyable() = default;
			virtual ~NonCopyable() = default;

		public:
			NonCopyable(const NonCopyable&) = delete;
			NonCopyable(NonCopyable&&) noexcept = default;
			NonCopyable& operator=(const NonCopyable&) = delete;
			NonCopyable& operator=(NonCopyable&&) noexcept = default;
	};
}

#endif
