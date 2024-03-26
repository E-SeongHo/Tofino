#pragma once


enum MOUSE_BUTTON
{
	LEFT_CLICK,
	RIGHT_CLICK,
	MIDDLE_CLICK
};

class Input
{
public:
	// Returns keyboard states
	bool IsKeyPressed(unsigned int keyCode);

	// Sets keyboard status
	void KeyPressed(unsigned int keyCode);
	void KeyUp(unsigned int keyCode);

	// Returns mouse states
	bool IsMouseButtonPressed(MOUSE_BUTTON btn) const;
	int GetMouseX() const;
	int GetMouseY() const;
	int GetMouseWheelAndReset();

	// Sets mouse status
	void MouseMoved(int mouseX, int mouseY);
	void MouseClicked(MOUSE_BUTTON btn);
	void MouseUp(MOUSE_BUTTON btn);
	void MouseScrolled(int wheel);

	// Sets cursor position at the center of the given window
	void CenterCursor(HWND window, int windowWidth, int windowHeight);

private:
	// Keyboard states, following windows keycode
	bool m_keyState[256] = { false };
	
	// Mouse status
	int m_mouseX;
	int m_mouseY;
	int m_scroll;

	// Mouse left, right, middle button state
	bool m_mouseBtnState[3] = { false }; 
};
