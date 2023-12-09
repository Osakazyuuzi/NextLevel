#include "Application.h"

#include <iostream>

#include "../../platform/WindowManager.h"

#include "src/engine/graphics/windows/Graphics.h"
#include "src/platform/windows/WindowsWindow.h"

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

namespace NextLevel
{
	/**
	* @brief ���������������܂��B
	*/
	void Application::Init()
	{
		m_window = WindowManager::CreateWindowInstanceForPlatform();
		m_window->Create(1280, 720, "Test");

		graphics::Graphics::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle(), m_window->GetWidth(), m_window->GetHeight());
	}

	/**
	* @brief ���������s���܂��B
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
	* @brief ���t���[�����������s���܂��B
	* @param _deltaTime �O�t���[������̌o�ߎ��ԁB
	*/
	void Application::DoFrame(float _deltaTime)
	{
		graphics::Graphics::GetInstance().BeginRender(0.0f, 0.5f, 0.5f);

		graphics::Graphics::GetInstance().EndRender();
	}

}