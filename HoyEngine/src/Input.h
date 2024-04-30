#pragma once

namespace Tofino
{
	enum MOUSE_BUTTON
	{
		LEFT_CLICK,
		RIGHT_CLICK,
		MIDDLE_CLICK
	};

	class Input
	{
	public:
		static Input& GetInstance()
		{
			static Input s;
			return s;
		}
		Input(const Input& other) = delete;
		Input& operator=(const Input& other) = delete;

		static bool IsKeyPressed(const unsigned int keyCode)		{ return GetInstance().m_keyState[keyCode]; }

		static void KeyPressed(const unsigned int keyCode)			{ GetInstance().m_keyState[keyCode] = true; }
		static void KeyUp(const unsigned int keyCode)				{ GetInstance().m_keyState[keyCode] = false; }

		static bool IsMouseButtonPressed(const MOUSE_BUTTON btn){ return GetInstance().m_mouseBtnState[btn]; }
		static int GetMouseX() 									{ return GetInstance().m_mouseX; }
		static int GetMouseY() 									{ return GetInstance().m_mouseY; }
		static int GetMouseWheelAndReset();

		static void MouseMoved(const int mouseX, const int mouseY)	{ GetInstance().m_mouseX = mouseX; GetInstance().m_mouseY = mouseY; }
		static void MouseClicked(const MOUSE_BUTTON btn)			{ GetInstance().m_mouseBtnState[btn] = true; }
		static void MouseUp(const MOUSE_BUTTON btn)					{ GetInstance().m_mouseBtnState[btn] = false; }
		static void MouseScrolled(const int wheel)					{ GetInstance().m_scroll = wheel; }

		// Sets cursor position at the center of the given window
		static void CenterCursor(HWND window, const int windowWidth, const int windowHeight);

	private:
		Input() = default;

		// Keyboard states, following windows keycode
		bool m_keyState[256] = { false };

		int m_mouseX;
		int m_mouseY;
		int m_scroll;

		bool m_mouseBtnState[3] = { false };
	};
}