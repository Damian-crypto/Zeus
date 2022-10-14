#include "corepch.h"
#include "event.h"

namespace zeus
{
	Event::Event()
	{
	}

	Event::Event(EventType& type)
		: m_Type(type)
	{
	}

	EventType Event::GetType()
	{
		return m_Type;
	}

	EventDispatcher::EventDispatcher(Event& evt)
		: m_Event(evt)
	{
	}

	bool EventDispatcher::Dispatch(EventType type, EventFunction& func)
	{
		// This event type checking is done to stop repeating same event calls
		if (m_Event.GetType() == type)
		{
			m_Event.m_Handled = func();
			return true;
		}

		return false;
	}
}
