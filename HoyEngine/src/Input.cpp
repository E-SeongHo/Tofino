
#include <Windows.h>

#include "Input.h"
#include <iostream>

namespace Tofino
{
	void Input::CenterCursor(HWND window, const int windowWidth, const int windowHeight)
	{
		POINT center = { windowWidth / 2, windowHeight / 2 };

		GetInstance().m_mouseX = center.x;
		GetInstance().m_mouseY = center.y;

		ClientToScreen(window, &center);
		SetCursorPos(center.x, center.y);
	}

	int Input::GetMouseWheelAndReset()
	{
		int ret = GetInstance().m_scroll;
		GetInstance().m_scroll = 0;
		return ret;
	}

}