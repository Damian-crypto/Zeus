#pragma once

#include "event.h"
#include "glm/glm.hpp"

namespace zeus
{
    class WindowResizedEvent : public Event
    {
    protected:
        glm::ivec2 m_WindowSize;

    public:
        WindowResizedEvent(int width, int height);

        glm::ivec2 GetWindowSize();
    };
}
