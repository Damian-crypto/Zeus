#include "zeus.h"

int main()
{
	auto app = new zeus::Application({ "Zeus Test Navigator" });
	app->Init();
	app->Run();

	return 0;
}
