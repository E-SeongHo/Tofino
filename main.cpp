#include <iostream>
#include <Windows.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "SystemBase.h"


int main()
{
	SystemBase app;
	if (!app.Init())
	{
		std::cout << "Init Failed" << std::endl;
		return -1;
	}

	app.Run();
	
	return 0;
}
