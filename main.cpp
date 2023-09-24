#include <iostream>
#include <Windows.h>

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
