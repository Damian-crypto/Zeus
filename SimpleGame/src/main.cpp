#include "zeus.h"

#include "glm/glm.hpp"

#include "game_layer.h"

int main()
{
	try
	{
		auto app = new zeus::Application({ "Jungle game", WIDTH, HEIGHT });
		app->Init();
		app->PushLayer(new GameLayer("jungle_game"));
		app->Run();
	}
	catch (std::exception e)
	{
		std::cerr << e.what() << std::endl;
	}
}
