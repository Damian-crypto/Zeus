#pragma once

#include "event.h"

namespace zeus
{
	class KeyPressedEvent : public Event
	{
	protected:
		int m_Keycode;

	public:
		KeyPressedEvent(int keycode)
			: m_Keycode(keycode)
		{
			m_Type = EventType::KeyPressed;
		}

		int GetKeycode()
		{
			return m_Keycode;
		}
	};

	class KeyReleasedEvent : public Event
	{
	protected:
		int m_Keycode;

	public:
		KeyReleasedEvent(int keycode)
			: m_Keycode(keycode)
		{
			m_Type = EventType::KeyReleased;
		}

		int GetKeycode()
		{
			return m_Keycode;
		}
	};
}
