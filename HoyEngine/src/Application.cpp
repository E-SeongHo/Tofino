
#include <iostream>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <tuple>

#include "Application.h"
#include "Scene.h"
#include "ShaderManager.h"

#include <directxtk/SimpleMath.h>
using namespace DirectX::SimpleMath;

using namespace std;


Application* pInstance = nullptr;
//https://stackoverflow.com/questions/17221815/why-cant-my-wndproc-be-in-a-class

Application::Application()
{
	pInstance = this;
	m_input = make_unique<Input>();

	// Early Initialization of Singleton
	Graphics::GetInstance();
	ShaderManager::GetInstance();
}

Application::~Application()
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

bool Application::Init()
{
	if (!InitWindow())	
		return false;

	if (!InitRenderer())
		return false;

	if (!InitGUI())
		return false;

	return true;
}


bool Application::InitWindow()
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
		L"Tofino Project",
		NULL
	};
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, m_width, m_height };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	// Create the window
	m_window = CreateWindow(
		wc.lpszClassName,
		L"Tofino : My Custom Engine",
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

bool Application::InitRenderer()
{
	//m_renderer = make_unique<Graphics>(m_window, m_width, m_height);
	
	if(!Graphics::Init(m_window, m_width, m_height))
		return false;

	return true;
}

bool Application::InitGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(float(m_width), float(m_height));

	ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	if (!Graphics::SetupGUIBackEnd())
		return false;

	if (!ImGui_ImplWin32_Init(m_window)) 
		return false;

	return true;
}

void Application::SetCurrentScene(Scene* scene)
{
	m_currentScene = scene;
}

void Application::Run()
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

			RenderGUI();
			ImGui::Render();

			Update(ImGui::GetIO().DeltaTime);
			Graphics::RenderScene(m_currentScene);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); // GUI 렌더링
			
			Graphics::Present();
		}
	}
}

void Application::Update(float deltaTime)
{
	switch (m_appMode)
	{
	case AppMode::EDIT:
		MouseTranslateObject(deltaTime);
		break;
	case AppMode::TRAVERSAL:
		AdjustSceneCamera(deltaTime);
		m_input->CenterCursor(m_window, m_width, m_height);
		break;
	}

	m_currentScene->Update(deltaTime);
}

void Application::ChangeAppMode(AppMode appMode)
{
	static AppMode defaultMode = AppMode::EDIT;
	
	if (m_appMode == appMode) return;
	
	m_appMode = appMode;
	if (m_appMode == AppMode::TRAVERSAL)
	{
		m_input->CenterCursor(m_window, m_width, m_height);
		ShowCursor(false);
	}
	else
	{
		ShowCursor(true);
	}
}

void Application::HandleKeyboardEvent() 
{
	if (m_input->IsKeyPressed(VK_ESCAPE)) PostMessage(m_window, WM_DESTROY, 0, 0);
	if (m_input->IsKeyPressed('F')) ChangeAppMode(AppMode::TRAVERSAL);
	if (m_input->IsKeyPressed('E')) ChangeAppMode(AppMode::EDIT);
}

void Application::MouseTranslateObject(float deltaTime)
{
    using namespace DirectX::SimpleMath;

    //cout << "wheel state : " << m_input->GetMouseWheel() << endl;

    static bool picking = false;
    static Object* pickingObject = nullptr;
    static Plane draggingPlane = Plane();
    static Vector3 prevHit = Vector3();

    int xPos = m_input->GetMouseX(); int yPos = m_input->GetMouseY();

    const float ndcX = (xPos * 2.0f / m_width) - 1.0f;
    const float ndcY = -((yPos * 2.0f / m_height) - 1.0f);

    Camera* cam = m_currentScene->GetCamera();

    if (m_input->IsMouseButtonPressed(MOUSE_BUTTON::LEFT_CLICK))
    {
        if (picking && pickingObject != nullptr) // on same plane
        {
            Matrix view = cam->GetViewMatrix();
            Matrix projection = cam->GetProjectionMatrix();
            Matrix unprojection = projection.Invert() * view.Invert();

            Vector3 p0 = Vector3(ndcX, ndcY, 0.0f);
            Vector3 p1 = Vector3(ndcX, ndcY, 1.0f);

            p0 = Vector3::Transform(p0, unprojection);
            p1 = Vector3::Transform(p1, unprojection);
            Vector3 direction = p1 - p0;
            direction.Normalize();

            Ray ray = Ray(p0, direction);
            float distance = 0.0f;
            ray.Intersects(draggingPlane, distance);

            Vector3 hitPoint = p0 + direction * distance;
            Vector3 dv = hitPoint - prevHit;

            pickingObject->UpdateWorldMatrix(pickingObject->GetWorldMatrix() * Matrix::CreateTranslation(dv));
            pickingObject->m_transform.Location += dv;
            pickingObject->UpdateBuffer(Graphics::GetDeviceContext());
            pickingObject->m_boundingSphere.Center = pickingObject->m_boundingSphere.Center + dv;

            prevHit = hitPoint;

			// Translate depth 
			int wheel = m_input->GetMouseWheelAndReset();
			if (wheel != 0 && picking && pickingObject != nullptr)
			{
				const int speed = 2;
				int movement = wheel / 120;
				Vector3 dv = pickingObject->m_boundingSphere.Center - cam->GetOrigin();
				dv.Normalize();
				dv = dv * movement * speed;

				Matrix m = Matrix::CreateTranslation(dv);
				pickingObject->UpdateWorldMatrix(pickingObject->GetWorldMatrix() * m);
				pickingObject->m_transform.Location += dv;
				pickingObject->UpdateBuffer(Graphics::GetDeviceContext());
				pickingObject->m_boundingSphere.Center = pickingObject->m_boundingSphere.Center + dv;

				Vector3 n = -cam->GetDirection();
				n.Normalize();
				draggingPlane = Plane(prevHit + dv, n);

				prevHit = prevHit + dv;
			}
        }
        else
        {
            Vector3 p0 = Vector3(ndcX, ndcY, 0.0f);
            Vector3 p1 = Vector3(ndcX, ndcY, 1.0f);
            // NDC -> View -> World
            Matrix unprojection = cam->GetProjectionMatrix().Invert() * cam->GetViewMatrix().Invert();

            p0 = Vector3::Transform(p0, unprojection);
            p1 = Vector3::Transform(p1, unprojection);

            Vector3 direction = p1 - p0;
            direction.Normalize();

            Ray ray = Ray(p0, direction);

            for (auto& object : m_currentScene->GetAllSceneObjects())
            {
                if (!object->hitEnabled) continue;
                float distance = 0.0f;
                bool hit = ray.Intersects(object->m_boundingSphere, distance);

                if (hit)
                {
                    if (!picking)
                    {
                        picking = true;
                        pickingObject = object;
                    }
                    Vector3 hitPoint = p0 + direction * distance;
                    Vector3 n = -cam->GetDirection();
                    n.Normalize();

                    prevHit = hitPoint;
                    draggingPlane = Plane(hitPoint, n);

                    break;
                }
            }
        }
    }
    else
    {
        picking = false;
        pickingObject = nullptr;
    }
}

void Application::AdjustSceneCamera(float deltaTime)
{
    Camera* cam = m_currentScene->GetCamera();

    cam->SetRunVars(m_input->IsKeyPressed(VK_SHIFT));

    if (m_input->IsKeyPressed('W'))
    {
        cam->MoveForward(deltaTime);
    }
    if (m_input->IsKeyPressed('A'))
    {
        cam->MoveRight(-deltaTime);
    }
    if (m_input->IsKeyPressed('S'))
    {
        cam->MoveForward(-deltaTime);
    }
    if (m_input->IsKeyPressed('D'))
    {
        cam->MoveRight(deltaTime);
    }

	int xPos = m_input->GetMouseX(); int yPos = m_input->GetMouseY();

	const float ndcX = (xPos * 2.0f / m_width) - 1.0f;
	const float ndcY = -((yPos * 2.0f / m_height) - 1.0f);

	cam->RotateFromMouse(ndcX, ndcY);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT Application::ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		// currentScene->GetCamera()->SetAspect(...)
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu 
			return 0;
		break;
	case WM_MOUSEMOVE:
		m_input->MouseMoved((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
		break;
	case WM_LBUTTONDOWN:
		m_input->MouseClicked(MOUSE_BUTTON::LEFT_CLICK);
		break;
	case WM_LBUTTONUP:
		m_input->MouseUp(MOUSE_BUTTON::LEFT_CLICK);
		break;
	case WM_MOUSEWHEEL:
		//cout << GET_WHEEL_DELTA_WPARAM(wParam) << endl;
		m_input->MouseScrolled(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_RBUTTONDOWN:
		m_input->MouseClicked(MOUSE_BUTTON::RIGHT_CLICK);
		break;
	case WM_RBUTTONUP:
		m_input->MouseUp(MOUSE_BUTTON::RIGHT_CLICK);
		break;
	case WM_KEYDOWN:
		m_input->KeyPressed(wParam);
		break;
	case WM_KEYUP:
		m_input->KeyUp(wParam);
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	HandleKeyboardEvent();

	return ::DefWindowProc(hWnd, msg, wParam, lParam);

}

