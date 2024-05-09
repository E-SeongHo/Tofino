
#include <iostream>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Application.h"
#include "SimpleMath.h"
#include "Scene.h"
#include "ShaderManager.h"
#include "Camera.h"
#include "Object.h"
#include "Input.h"
#include "Graphics.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Tofino
{
	Application* pInstance = nullptr;
	//https://stackoverflow.com/questions/17221815/why-cant-my-wndproc-be-in-a-class

	Application::Application()
		: m_editorCamera(std::make_unique<Camera>())
	{
		pInstance = this;

		m_editorCamera->SetAspect(static_cast<float>(m_width) / m_height);

		// Early Initialization of Singletons
		Graphics::GetInstance();
		Input::GetInstance();
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

	bool Application::InitRenderer() const
	{
		if (!RendererInstance.Init(m_window, m_width, m_height))
			return false;

		return true;
	}

	bool Application::InitGUI() const
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.DisplaySize = ImVec2(float(m_width), float(m_height));

		ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		if (!RendererInstance.SetupGUIBackEnd())
			return false;

		if (!ImGui_ImplWin32_Init(m_window))
			return false;

		return true;
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
				ImGui_ImplDX11_NewFrame(); 
				ImGui_ImplWin32_NewFrame();

				RenderGUI();
				ImGui::Render();

				Update(ImGui::GetIO().DeltaTime);
				RendererInstance.RenderScene(m_currentScene);
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				RendererInstance.Present();
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
			AdjustEditorCamera(deltaTime);
			Input::CenterCursor(m_window, m_width, m_height);
			break;
		case AppMode::PLAY:
			break;
		}

		if(m_appMode != AppMode::PLAY) // use default App's editor camera
		{	
			auto& sceneConst = m_currentScene->GetSceneConstBuffer().GetData();

			// Updates Scene Buffer every frame
			sceneConst.eye = m_editorCamera->GetOrigin();
			sceneConst.view = m_editorCamera->GetViewMatrix().Transpose();
			sceneConst.projection = m_editorCamera->GetProjectionMatrix().Transpose();
			m_currentScene->GetSceneConstBuffer().Update(RendererContext);
		}

		m_currentScene->Update(deltaTime);
	}

	void Application::ChangeAppMode(AppMode appMode)
	{
		static AppMode defaultMode = AppMode::EDIT;

		if (m_appMode == appMode) return;

		m_appMode = appMode;

		switch(m_appMode)
		{
		case AppMode::EDIT:
			m_currentScene->SetPlay(false);
			ShowCursor(true);
			break;
		case AppMode::TRAVERSAL:
			m_currentScene->SetPlay(false);
			Input::CenterCursor(m_window, m_width, m_height);
			ShowCursor(false);
			break;
		case AppMode::PLAY:
			m_currentScene->SetPlay(true);
			Input::CenterCursor(m_window, m_width, m_height);
			//ShowCursor(false); // temporary commented
			break;
		}
	}

	void Application::HandleKeyboardEvent()
	{
		if (Input::IsKeyPressed(VK_ESCAPE)) PostMessage(m_window, WM_DESTROY, 0, 0);
		if (Input::IsKeyPressed('F')) ChangeAppMode(AppMode::TRAVERSAL);
		if (Input::IsKeyPressed('E')) ChangeAppMode(AppMode::EDIT);
		if (Input::IsKeyPressed('P')) ChangeAppMode(AppMode::PLAY);
	}

	void Application::MouseTranslateObject(float deltaTime)
	{
		//cout << "wheel state : " << Input::GetMouseWheel() << endl;

		static bool picking = false;
		static Object* pickingObject = nullptr;
		static Plane draggingPlane = Plane();
		static Vector3 prevHit = Vector3();

		int xPos = Input::GetMouseX(); int yPos = Input::GetMouseY();

		const float ndcX = (xPos * 2.0f / m_width) - 1.0f;
		const float ndcY = -((yPos * 2.0f / m_height) - 1.0f);

		if (Input::IsMouseButtonPressed(MOUSE_BUTTON::LEFT_CLICK))
		{
			if (picking && pickingObject != nullptr) // on same plane
			{
				Matrix view = m_editorCamera->GetViewMatrix();
				Matrix projection = m_editorCamera->GetProjectionMatrix();
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

				pickingObject->GetComponent<TransformComponent>().Translation += dv;
				pickingObject->SetUpdateFlag(true);
				prevHit = hitPoint;

				// Translate depth 
				int wheel = Input::GetMouseWheelAndReset();
				if (wheel != 0 && picking && pickingObject != nullptr)
				{
					constexpr int speed = 2;
					int movement = wheel / 120;
					Vector3 dv = pickingObject->m_boundingSphere.Center - m_editorCamera->GetOrigin();
					dv.Normalize();
					dv = dv * movement * speed;

					pickingObject->GetComponent<TransformComponent>().Translation += dv;
					pickingObject->SetUpdateFlag(true);

					Vector3 n = -m_editorCamera->GetDirection();
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
				Matrix unprojection = m_editorCamera->GetProjectionMatrix().Invert() * m_editorCamera->GetViewMatrix().Invert();

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
							pickingObject = object.get();
						}
						Vector3 hitPoint = p0 + direction * distance;
						Vector3 n = -m_editorCamera->GetDirection();
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

	void Application::AdjustEditorCamera(float deltaTime)
	{
		//Camera* cam = m_currentScene->GetCamera();

		m_editorCamera->SetRunVars(Input::IsKeyPressed(VK_SHIFT));

		if (Input::IsKeyPressed('W'))
		{
			m_editorCamera->MoveForward(deltaTime);
		}
		if (Input::IsKeyPressed('A'))
		{
			m_editorCamera->MoveRight(-deltaTime);
		}
		if (Input::IsKeyPressed('S'))
		{
			m_editorCamera->MoveForward(-deltaTime);
		}
		if (Input::IsKeyPressed('D'))
		{
			m_editorCamera->MoveRight(deltaTime);
		}

		int xPos = Input::GetMouseX(); int yPos = Input::GetMouseY();

		const float ndcX = (xPos * 2.0f / m_width) - 1.0f;
		const float ndcY = -((yPos * 2.0f / m_height) - 1.0f);

		m_editorCamera->RotateFromMouse(ndcX, ndcY);
	}

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
			Input::MouseMoved((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
			break;
		case WM_LBUTTONDOWN:
			Input::MouseClicked(MOUSE_BUTTON::LEFT_CLICK);
			break;
		case WM_LBUTTONUP:
			Input::MouseUp(MOUSE_BUTTON::LEFT_CLICK);
			break;
		case WM_MOUSEWHEEL:
			//cout << GET_WHEEL_DELTA_WPARAM(wParam) << endl;
			Input::MouseScrolled(GET_WHEEL_DELTA_WPARAM(wParam));
			break;
		case WM_RBUTTONDOWN:
			Input::MouseClicked(MOUSE_BUTTON::RIGHT_CLICK);
			break;
		case WM_RBUTTONUP:
			Input::MouseUp(MOUSE_BUTTON::RIGHT_CLICK);
			break;
		case WM_KEYDOWN:
			Input::KeyPressed(wParam);
			break;
		case WM_KEYUP:
			Input::KeyUp(wParam);
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			return 0;
		}

		HandleKeyboardEvent();

		return ::DefWindowProc(hWnd, msg, wParam, lParam);

	}

}