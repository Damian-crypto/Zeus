#include "zeus.h"

#include <iostream>

#include "glm/glm.hpp"
#include "game_layer.h"

int main()
{
	try
	{
		auto app = new zeus::Application({ "City game", WIDTH, HEIGHT });
		app->Init();
		app->SetVSync(true);
		app->PushLayer(new GameLayer("city_game"));
		app->Run();
	}
	catch (std::exception e)
	{
		LOG(Error, e.what());
	}
}
