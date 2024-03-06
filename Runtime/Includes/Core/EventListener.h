#ifndef __YAVR_CORE_EVENT_LISTENER__
#define __YAVR_CORE_EVENT_LISTENER__

#include <PreCompiled.h>
#include <Core/EventBase.h>

namespace Yavr
{
	class EventListener
	{
		public:
			EventListener() = delete;
			EventListener(std::function<void(const EventBase&)> functor, std::string name);

			inline const std::string& GetName() const { return m_name; }
			inline void Call(const EventBase& event) const noexcept { m_listen_functor(event); }

			~EventListener() = default;

		private:
			std::function<void(const EventBase&)> m_listen_functor;
			std::string m_name;
	};
}

#endif
