
#include <iostream>

#include "SystemBase.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

using namespace std;

bool SystemBase::Init()
{
	if (!InitWindow())	
		return false;

	if (!InitRenderer())
		return false;

	return true;
}


bool SystemBase::InitWindow()
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"HoyEngine",
		NULL
	};
	RegisterClassEx(&wc);

	int posX = 100, posY = 100;

	RECT wr = { 0, 0, m_width, m_height };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// Create the window
	m_window = CreateWindow(
		wc.lpszClassName,
		L"Hoy-Engine : MY CUSTOM ENGINE",
		WS_OVERLAPPEDWINDOW,
		posX,
		posY,
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도    
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	ShowWindow(m_window, SW_SHOWDEFAULT);
	UpdateWindow(m_window);

	return true;
}

bool SystemBase::InitRenderer()
{
	m_renderer = make_unique<Graphics>(m_window, m_width, m_height);
	
	if(!m_renderer->Init())
		return false;

	return true;
}

void SystemBase::Run()
{
	MSG msg = {};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_renderer->Render();
			m_renderer->m_swapChain->Present(1, 0);
		}
	}
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;*/

	switch (msg)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu 
			return 0;
		break;
	case WM_MOUSEMOVE:
		//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		break;
	case WM_LBUTTONUP:
		//std::cout << "WM_LBUTTONUP Left mouse button" << std::endl;
		break;
	case WM_RBUTTONUP:
		//std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
		break;
	case WM_KEYDOWN:
		std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);

}