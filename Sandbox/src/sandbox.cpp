#include "sandbox.h"
#include "sandbox_layer.h"
#include "engine_layer.h"

int main()
{
	auto app = new zeus::Application({ "Sandbox", WIDTH, HEIGHT, false });
	app->Init();
	app->PushLayer(new SandboxLayer("sandbox"));
	app->PushLayer(new EngineInfoLayer("engine"));
	app->Run();

	return 0;
}
