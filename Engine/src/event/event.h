#pragma once

#include <functional>

namespace zeus
{
	enum class EventType
	{
		None,

		MousePressed,
		MouseReleased,
		MouseMoved,
		MouseScrolled,

		KeyPressed,
		KeyReleased
	};

	class Event
	{
	protected:
		EventType m_Type = EventType::None;

	public:
		bool m_Handled = false;

	public:
		Event();
		Event(EventType& type);
		
		EventType GetType();
	};

	class EventDispatcher
	{
	private:
		Event m_Event;

		using EventFunction = std::function<bool()>;

	public:
		EventDispatcher(Event& evt);

		bool Dispatch(EventType type, EventFunction& func);
	};
}
