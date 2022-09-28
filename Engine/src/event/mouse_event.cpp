#include "mouse_event.h"

namespace zeus
{
	MousePressedEvent::MousePressedEvent(int button)
		: m_Button(button)
	{
		m_Type = EventType::MousePressed;
	}

	uint32_t MousePressedEvent::GetMouseButton()
	{
		return m_Button;
	}

	MouseMovedEvent::MouseMovedEvent(float x, float y)
		: m_Xpos(x), m_Ypos(y)
	{
		m_Type = EventType::MouseMoved;
	}

	glm::vec2 MouseMovedEvent::GetMousePosition()
	{
		return { m_Xpos, m_Ypos };
	}
}
