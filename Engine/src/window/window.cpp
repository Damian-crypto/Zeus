#include "window.h"

#ifdef WINDOWING_SYSTEM_GLFW
#include "windows_window.h"
#endif

namespace zeus
{
    std::shared_ptr<Window> Window::GetWindow()
    {
#ifdef WINDOWING_SYSTEM_GLFW
		return std::make_shared<WindowsWindow>();
#else
        return std::make_shared<LinuxWindow>();
#endif
    }
}
