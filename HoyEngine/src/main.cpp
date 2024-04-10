#include <iostream>
#include <Windows.h>

#include "Application.h"

extern Tofino::Application* CreateApplication();

int main(int argc, char** argv)
{
	Tofino::Application* app = CreateApplication();

	if (!app->Init())
	{
		std::cout << "Init Failed" << std::endl;
		return -1;
	}

	app->Run();

	return 0;
}

