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

	public:
		void Init();
		void Begin();
		void End();
		void Destroy();

		void AddMenuItem(const MenuItem& item);

		static bool IsBlocking();
	};
}
