#ifndef __YAVR_CORE_EVENT_BUS__
#define __YAVR_CORE_EVENT_BUS__

#include <PreCompiled.h>
#include <Core/EventBase.h>
#include <Core/EventListener.h>

namespace Yavr
{
	class EventBus
	{
		public:
			EventBus() = delete;
			static void Send(const std::string& listener_name, const EventBase& event);
			inline static void RegisterListener(const EventListener& listener) { s_listeners.push_back(listener); }
			~EventBus() = delete;

		private:
			inline static std::vector<EventListener> s_listeners;
	};
}

#endif
