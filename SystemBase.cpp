
#include <iostream>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <tuple>

#include "SystemBase.h"


using namespace std;

SystemBase* pInstance = nullptr;
//https://stackoverflow.com/questions/17221815/why-cant-my-wndproc-be-in-a-class

SystemBase::~SystemBase()
{
	pInstance = nullptr;
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(m_window);
}

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return pInstance->ProcessMessage(hWnd, msg, wParam, lParam);
}

bool SystemBase::Init()
{
	pInstance = this;

	if (!InitWindow())	
		return false;

	if (!InitRenderer())
		return false;

	if (!InitGUI())
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

	RECT wr = { 0, 0, m_width, m_height };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// Create the window
	m_window = CreateWindow(
		wc.lpszClassName,
		L"Hoy-Engine : MY CUSTOM ENGINE",
		WS_OVERLAPPEDWINDOW,
		m_startX,
		m_startY,
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

bool SystemBase::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.DisplaySize = ImVec2(float(m_width), float(m_height));

	// Setup Platform/Renderer backends
	if (!m_renderer->SetupGUIBackEnd())
		return false;

	if (!ImGui_ImplWin32_Init(m_window)) 
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
			ImGui_ImplDX11_NewFrame(); // GUI 프레임 시작
			ImGui_ImplWin32_NewFrame();

			m_renderer->RenderGUI(); 

			ImGui::Render(); // 렌더링할 것들 기록 끝

			m_renderer->Update(ImGui::GetIO().DeltaTime);
			m_renderer->Render();

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링
			
			m_renderer->Present();
		}
	}
}


void SystemBase::ProcessKeyboardInput(HWND hWnd, WPARAM keyPress)
{
	switch (keyPress)
	{
	case VK_ESCAPE:
		PostMessage(hWnd, WM_DESTROY, 0, 0);
		break;
	case 0x46: // 'F' key
		m_renderer->ToggleFPVMode();
		break;
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT SystemBase::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

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
		m_renderer->ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		//cout << wParam << endl;
		m_renderer->m_leftClick = true;
		m_renderer->ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONUP:
		m_renderer->m_leftClick = false;
		break;
	case WM_MOUSEWHEEL:
		//cout << GET_WHEEL_DELTA_WPARAM(wParam) << endl;
		m_renderer->ProcessMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_RBUTTONUP:
		//std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
		break;
	case WM_KEYDOWN:
		// std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		m_renderer->m_keyState[wParam] = true;
		ProcessKeyboardInput(hWnd, wParam);
		break;
	case WM_KEYUP:
		m_renderer->m_keyState[wParam] = false;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);

}

