#pragma once

#include "core.h"

namespace zeus
{
	struct WindowProperties
	{
		std::string Title{""};
		uint32_t Width{1280};
		uint32_t Height{720};
	};

	class Window
	{
	protected:
		WindowProperties m_Properties;

	public:
		virtual void CreateWindow(WindowProperties props) = 0;
		virtual void SwapBuffers() = 0;
		virtual void ProcessEvents() = 0;
		virtual float GetTime() = 0;
		virtual bool ShouldClose() = 0;
		virtual void* GetNativeWindow() = 0;
		virtual void TerminateWindow() = 0;
		virtual void VSyncEnable(bool enabled) = 0;
		virtual void SetWindowTitle(const std::string& title) = 0;
		virtual void SetCameraMode(bool enabled) = 0;

		static std::shared_ptr<Window> GetWindow();
	};
}
