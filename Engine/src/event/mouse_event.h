#pragma once

#include "event.h"
#include "glm/glm.hpp"

namespace zeus
{
	class MousePressedEvent : public Event
	{
	protected:
		int m_Button;

	public:
		MousePressedEvent(int button);

		uint32_t GetMouseButton();
	};

	class MouseMovedEvent : public Event
	{
	protected:
		float m_Xpos;
		float m_Ypos;

	public:
		MouseMovedEvent(float x, float y);

		glm::vec2 GetMousePosition();
	};
}
