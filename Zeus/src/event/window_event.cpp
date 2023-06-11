#include "corepch.h"
#include "window_event.h"

namespace zeus
{
    WindowResizedEvent::WindowResizedEvent(int width, int height)
    {
        m_Type = EventType::WindowResized;
        m_WindowSize = { width, height };
    }

    glm::ivec2 WindowResizedEvent::GetWindowSize()
    {
        return m_WindowSize;
    }
}
