#pragma once

struct GLFWwindow;

namespace zeus
{
	class ImGUI
	{
	private:
		GLFWwindow* m_NativeWindow;

	public:
		void Init();
		void Begin();
		void End();
		void Destroy();

		static bool IsBlocking();
	};
}
