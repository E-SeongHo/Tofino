
#include <Windows.h>

#include "Input.h"
#include <iostream>

bool Input::IsKeyPressed(unsigned int keyCode) 
{
	return m_keyState[keyCode];
}

void Input::KeyPressed(unsigned int keyCode)
{
	m_keyState[keyCode] = true;
}

void Input::KeyUp(unsigned int keyCode) 
{
	m_keyState[keyCode] = false;
}

bool Input::IsMouseButtonPressed(MOUSE_BUTTON btn) const
{
	return m_mouseBtnState[btn];
}

int Input::GetMouseX() const
{
	return m_mouseX;
}

void Input::CenterCursor(HWND window, int windowWidth, int windowHeight)
{
	POINT center = { windowWidth / 2, windowHeight / 2 };
	
	m_mouseX = center.x;
	m_mouseY = center.y;

	ClientToScreen(window, &center);
	SetCursorPos(center.x, center.y);
}

int Input::GetMouseY() const
{
	return m_mouseY;
}

int Input::GetMouseWheelAndReset()
{
	int ret = m_scroll;
	m_scroll = 0;
	return ret;
}

void Input::MouseMoved(int mouseX, int mouseY)
{
	m_mouseX = mouseX;
	m_mouseY = mouseY;
}

void Input::MouseClicked(MOUSE_BUTTON btn)
{
	m_mouseBtnState[btn] = true;
}

void Input::MouseUp(MOUSE_BUTTON btn)
{
	m_mouseBtnState[btn] = false;
}

void Input::MouseScrolled(int wheel)
{
	m_scroll = wheel;
}