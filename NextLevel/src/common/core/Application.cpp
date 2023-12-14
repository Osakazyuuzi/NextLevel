#include "Application.h"

#include <iostream>

#include "../../platform/WindowManager.h"

#include "src/platform/windows/WindowsWindow.h"
#include "src/engine/graphics/windows/Graphics.h"
#include "src/engine/editor/windows/EditorManager.h"

#include "src/common/core/resource/shader/ShaderManager.h"
#include "src/common/core/resource/material/MaterialManager.h"
#include "src/common/core/resource/model/ModelManager.h"

#include "third_party/ImGui/imgui.h"
#include "third_party/ImGui/imgui_impl_dx11.h"
#include "third_party/ImGui/imgui_impl_win32.h"

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

namespace NextLevel
{
	/**
	* @brief ���������������܂��B
	*/
	void Application::Init()
	{
		// �E�B���h�E�̏�����
		m_window = WindowManager::CreateWindowInstanceForPlatform();
		m_window->Create(1280, 720, "NextLevel");

		// �O���t�B�b�N�X�̏�����
		graphics::Graphics::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle(), m_window->GetWidth(), m_window->GetHeight());

		// �G�f�B�^�̏�����
		EditorManager::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle());

		// �V�F�[�_�[�}�l�[�W���[�̃Z�[�u�f�[�^�ǂݍ���
		resource::ShaderManager::GetInstance().LoadFromJson();
		// �}�e���A���}�l�[�W���[�̃Z�[�u�f�[�^�ǂݍ���
		resource::MaterialManager::GetInstance().LoadFromJson();
		// ���f���}�l�[�W���[�̃Z�[�u�f�[�^�ǂݍ���
		resource::ModelManager::GetInstance().LoadFromJson();


		// ����\
		timeBeginPeriod(1);
	}

	void Application::Uninit()
	{
		timeEndPeriod(1);
		EditorManager::GetInstance().Shutdown();
		graphics::Graphics::GetInstance().Shutdown();
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