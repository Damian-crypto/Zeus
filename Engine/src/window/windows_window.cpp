#include "corepch.h"
#include "windows_window.h"

#include "event/window_event.h"
#include "event/mouse_event.h"
#include "event/key_event.h"
#include "util/logger.h"
#include "application.h"
#include "core.h"

namespace zeus
{
	void WindowsWindow::CreateWindow(WindowProperties props)
	{
		m_Properties = props;

		if (!glfwInit())
		{
			LOG_ENGINE(Error, "Runtime Error: Windowing system not initialized!");
		}

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if DEBUG_MODE
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		glfwWindowHint(GLFW_RESIZABLE, props.Resizable ? GL_TRUE : GL_FALSE);

		m_NativeWindow = glfwCreateWindow((int)m_Properties.Width, (int)m_Properties.Height, m_Properties.Title.c_str(), nullptr, nullptr);
		if (m_NativeWindow == nullptr)
		{
			LOG_ENGINE(Error, "Runtime Error: Window creation failed!");
		}

		glfwMakeContextCurrent(m_NativeWindow);

		if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
		{
			LOG_ENGINE(Error, "Runtime Error: OpenGL function binding failed!");
		}

#if DEBUG_MODE
		int contextFlags = 0;
		glGetIntegerv(GL_CONTEXT_FLAGS, &contextFlags);

		if (contextFlags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			LOG_ENGINE(Trace, "Debug context created!");
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback([](unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* msg, const void* user) {
				LOG_ENGINE(Warn, "[GLFW DEBUG]: %s\n", msg);
				}, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
#endif

		glfwSetMouseButtonCallback(m_NativeWindow, [](GLFWwindow* window, int button, int action, int mods) {
			switch (action)
			{
				case GLFW_PRESS:
				{
					MousePressedEvent evt(button);
					Application::GetInstance()->OnEvent(evt);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseReleasedEvent evt(button);
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

		glfwSetScrollCallback(m_NativeWindow, [](GLFWwindow* window, double x, double y) {
			MouseScrolledEvent evt((float)x, (float)y);
			Application::GetInstance()->OnEvent(evt);
		});

		glfwSetWindowSizeCallback(m_NativeWindow, [](GLFWwindow* window, int width, int height) {
			WindowResizedEvent evt(width, height);
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
			LOG_ENGINE(Error, "Runtime Error: Changing vsync before initializing native window!");
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

	void WindowsWindow::SetCameraMode(bool enabled)
	{
		if (enabled)
			glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(m_NativeWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
