#pragma once

#include <string>
#include <functional>
#include <vector>

struct GLFWwindow;

namespace zeus
{
	struct MenuItem
	{
		std::string ItemName;
		std::function<bool(void)> Action;
	};

	class ImGUI
	{
	private:
		GLFWwindow* m_NativeWindow;
		std::vector<MenuItem> m_MenuItems;
		bool m_GameUI = true;
		static bool m_Initialized;

	public:
		void Init(bool gameUI);
		void Begin();
		void End();
		void Destroy();

		std::string ShowOpenFileDialog();

		void AddMenuItem(const MenuItem& item);
		void SetGamingMode(bool gaming);

		static bool IsBlocking();
	};
}
