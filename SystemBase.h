#pragma once

#include <Windows.h>
#include <memory>
#include <ctime>

#include "Graphics.h"

class SystemBase
{
public:
	int m_width = 1600, m_height = 900;
	HWND m_window;
	std::unique_ptr<Graphics> m_renderer;

public:
	~SystemBase();
	bool Init();
	bool InitWindow();
	bool InitRenderer();
	bool InitGUI();

	void Run();

	void ProcessKeyboardInput(HWND hWnd, WPARAM keyPress);
	void ProcessMouseInput(int xPos, int yPos);

	LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};