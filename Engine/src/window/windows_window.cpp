#include "windows_window.h"

#include "event/mouse_event.h"
#include "event/key_event.h"
#include "application.h"

namespace zeus
{
	void WindowsWindow::CreateWindow(WindowProperties props)
	{
		m_Properties = props;

		if (!glfwInit())
		{
			__debugbreak();
		}

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_NativeWindow = glfwCreateWindow((int)m_Properties.Width, (int)m_Properties.Height, m_Properties.Title.c_str(), nullptr, nullptr);
		if (m_NativeWindow == nullptr)
		{
			__debugbreak();
		}

		glfwMakeContextCurrent(m_NativeWindow);

		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
		{
			__debugbreak();
		}

		glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* window, int button, int action, int mods) {
			switch (action)
			{
				case GLFW_PRESS:
				{
					MousePressedEvent evt(button);
					Application::GetInstance()->OnEvent(evt);
					break;
				}
			}
		});

		glfwSetKeyCallback(m_NativeWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent evt(key);
					Application::GetInstance()->OnEvent(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent evt(key);
					Application::GetInstance()->OnEvent(evt);
					break;
				}
			}
		});

		glfwSetFramebufferSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height) {
			auto& props = Application::GetInstance()->GetApplicationProperties();
			props.Width = (uint32_t)width;
			props.Height = (uint32_t)height;
			
			glViewport(0, 0, width, height);
		});

		glfwSetCursorPosCallback(m_NativeWindow, [](GLFWwindow* window, double x, double y) {
			MouseMovedEvent evt((float)x, (float)y);
			Application::GetInstance()->OnEvent(evt);
		});
	}

	void WindowsWindow::SwapBuffers()
	{
		glfwSwapBuffers(m_NativeWindow);
	}

	void WindowsWindow::ProcessEvents()
	{
		glfwPollEvents();
	}

	float WindowsWindow::GetTime()
	{
		return (float)glfwGetTime();
	}

	bool WindowsWindow::ShouldClose()
	{
		return glfwWindowShouldClose(m_NativeWindow);
	}

	void* WindowsWindow::GetNativeWindow()
	{
		return m_NativeWindow;
	}

	void WindowsWindow::TerminateWindow()
	{
		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}

	void WindowsWindow::VSyncEnable(bool enabled)
	{
		if (m_NativeWindow == nullptr)
		{
			throw std::runtime_error("Runtime Error: Changing vsync before initializing native window!");
		}

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void WindowsWindow::SetWindowTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_NativeWindow, title.c_str());
	}
}
