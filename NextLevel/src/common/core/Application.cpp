#include "Application.h"

#include <iostream>

#include "../../platform/WindowManager.h"

#include "src/platform/windows/WindowsWindow.h"
#include "src/engine/graphics/windows/Graphics.h"
#include "src/engine/editor/windows/EditorManager.h"

#include "third_party/ImGui/imgui.h"
#include "third_party/ImGui/imgui_impl_dx11.h"
#include "third_party/ImGui/imgui_impl_win32.h"

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

namespace NextLevel
{
	/**
	* @brief 初期化処理をします。
	*/
	void Application::Init()
	{
		m_window = WindowManager::CreateWindowInstanceForPlatform();
		m_window->Create(1280, 720, "NextLevel");

		graphics::Graphics::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle(), m_window->GetWidth(), m_window->GetHeight());

		EditorManager::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle());

		timeBeginPeriod(1);
	}

	void Application::Uninit()
	{
		timeEndPeriod(1);
		EditorManager::GetInstance().Shutdown();
		graphics::Graphics::GetInstance().Shutdown();
	}

	/**
	* @brief 処理を実行します。
	*/
	void Application::Run()
	{
		while (1)
		{
			if (!m_window->ProcessEvent())
			{
				break;
			}
			else
			{
				m_dwCurrentTime = timeGetTime();
				float deltaTime = m_dwCurrentTime - m_dwExecLastTime;
				if (deltaTime >= (1000 / (m_fFps * m_fTimeScale)))
				{
					DoFrame(deltaTime);
					m_dwExecLastTime = m_dwCurrentTime;
				}
			}
		}
	}

	/**
	* @brief 毎フレーム処理を実行します。
	* @param _deltaTime 前フレームからの経過時間。
	*/
	void Application::DoFrame(float _deltaTime)
	{
		graphics::Graphics::GetInstance().BeginRender(
			150.0f / 255.0f, 
			200.0f / 255.0f, 
			180.0f / 255.0f);
		EditorManager::GetInstance().BeginRender();

		EditorManager::GetInstance().Draw();


		EditorManager::GetInstance().EndRender();
		graphics::Graphics::GetInstance().EndRender();
	}

}