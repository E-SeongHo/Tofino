#pragma once

#include <Windows.h>
#include <memory>
#include <ctime>
#include <functional>

#ifndef NOMINMAX
	#define NOMINMAX
#endif

namespace Tofino
{
	class Graphics;
	class Scene;
	class Camera;

	enum class AppMode
	{
		EDIT, // default
		TRAVERSAL,
		PLAY
	};

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual bool Init();
		void Run();

		void SetCurrentScene(Scene* scene) { m_currentScene = scene; }

		virtual void Update(float deltaTime);
		virtual void RenderGUI() = 0;

		virtual void HandleKeyboardEvent();
		LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		bool InitWindow();
		bool InitRenderer() const;
		bool InitGUI() const;

		void ChangeAppMode(AppMode appMode);

		void MouseTranslateObject(float deltaTime);
		void AdjustEditorCamera(float deltaTime);

	protected:
		int m_width = 1600, m_height = 900;
		int m_startX = 100, m_startY = 100;

		HWND m_window;
		AppMode m_appMode = AppMode::EDIT;
		std::unique_ptr<Camera> m_editorCamera = nullptr;
		Scene* m_currentScene = nullptr;
	};

	// Defined in Client Project
	Application* CreateApplication();
}