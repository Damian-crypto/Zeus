#include "corepch.h"
#include "input.h"

#include "application.h"
#include "GLFW/glfw3.h"

namespace zeus
{
	std::pair<float, float> Input::GetMousePosition()
	{
		GLFWwindow* window = (GLFWwindow*)Application::GetInstance()->GetWindow()->GetNativeWindow();
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return { (float)xPos, (float)yPos };
	}
}
