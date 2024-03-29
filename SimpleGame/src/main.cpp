#include "zeus.h"

#include "glm/glm.hpp"
#include "game_layer.h"

#include <iostream>

int main()
{
	zeus::ApplicationProperties props;
	props.Title = "Survival";
	props.Width = WIDTH;
	props.Height = HEIGHT;
	props.GameUI = true;
	props.Resizable = false;

	auto app = new zeus::Application(props);
	app->Init();
	app->SetVSync(true);
	zeus::Layer* layer = new GameLayer("zeus_survival");
	app->PushLayer(layer);
	app->Run();
}
