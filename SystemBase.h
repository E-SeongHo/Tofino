#pragma once

#include <Windows.h>
#include <memory>
#include <ctime>

#include "Graphics.h"

class SystemBase
{
public:
	int m_width = 1280, m_height = 720;
	HWND m_window;
	std::unique_ptr<Graphics> m_renderer;


public:
	bool Init();
	bool InitWindow();
	bool InitRenderer();
	bool InitGUI();

	void Run();
};