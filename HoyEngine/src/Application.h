#pragma once

#include <Windows.h>
#include <memory>
#include <ctime>
#include <functional>

#include "Graphics.h"
#include "Input.h"

#ifndef NOMINMAX
	#define NOMINMAX
#endif

namespace Tofino
{
	enum class AppMode
	{
		EDIT, // default
		TRAVERSAL,
	};

	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual bool Init();

		void Run();

		// Sets current scene
		void SetCurrentScene(Scene* scene);

		virtual void Update(float deltaTime);

		virtual void RenderGUI() = 0;

		// Process keyboard events (key settings)
		virtual void HandleKeyboardEvent();

		LRESULT ProcessMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		bool InitWindow();
		bool InitRenderer();
		bool InitGUI();

		// Sets application mode
		void ChangeAppMode(AppMode appMode);

		// Translates picking objects with mouse
		void MouseTranslateObject(float deltaTime);

		// Moves and Rotates current scene camera
		void AdjustSceneCamera(float deltaTime);

	protected:
		int m_width = 1600, m_height = 900;
		int m_startX = 100, m_startY = 100;

		HWND m_window;
		std::unique_ptr<Input> m_input;
		AppMode m_appMode = AppMode::EDIT;

	private:
		Scene* m_currentScene = nullptr;

		//friend int main(int argc, char** argv);
	};

	// Defined in Client Project
	Application* CreateApplication();
}