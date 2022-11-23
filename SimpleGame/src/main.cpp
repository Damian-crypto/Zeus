#include "zeus.h"

#include "glm/glm.hpp"
#include "game_layer.h"

#include <iostream>

int main()
{
	try
	{
		auto app = new zeus::Application({ "City game", WIDTH, HEIGHT });
		app->Init();
		app->SetVSync(true);
		zeus::Layer* layer = new GameLayer("city_game");
		app->PushLayer(layer);
		app->Run();
	}
	catch (std::exception e)
	{
		LOG(Error, e.what());
	}
}
