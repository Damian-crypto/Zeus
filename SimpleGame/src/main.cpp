#include "zeus.h"

#include "glm/glm.hpp"
#include "game_layer.h"

#include <iostream>

int main()
{
	try
	{
		zeus::ApplicationProperties props {
			.Title = "City Game",
			.Width = WIDTH,
			.Height = HEIGHT,
			.GameUI = true,
			.Resizable = false
		};

		auto app = new zeus::Application(props);
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
