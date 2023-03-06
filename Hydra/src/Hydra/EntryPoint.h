#pragma once

extern Hydra::Application* Hydra::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Hydra::CreateApplication();
	app->Run();
	delete app;
	return 0;
}