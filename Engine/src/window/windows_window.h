#pragma once

#include "core.h"
#include "window.h"
#include "layer_queue.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace zeus
{
	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_NativeWindow = nullptr;

	public:
		void CreateWindow(WindowProperties props) override;
		void SwapBuffers() override;
		void ProcessEvents() override;
		float GetTime() override;
		bool ShouldClose() override;
		void* GetNativeWindow() override;
		void TerminateWindow() override;
		void VSyncEnable(bool active) override;
		void SetWindowTitle(const std::string& title) override;
		void SetCameraMode(bool enabled) override;
	};
}
