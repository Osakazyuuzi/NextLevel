#include "Application.h"

#include <iostream>

#include "../../platform/WindowManager.h"

#include "src/platform/windows/WindowsWindow.h"
#include "src/engine/graphics/windows/Graphics.h"
#include "src/engine/editor/windows/EditorManager.h"
#include "src/common/core/ecs/WorldManager.h"

#include "src/common/core/resource/shader/ShaderManager.h"
#include "src/common/core/resource/material/MaterialManager.h"
#include "src/common/core/resource/model/ModelManager.h"

#include "third_party/ImGui/imgui.h"
#include "third_party/ImGui/imgui_impl_dx11.h"
#include "third_party/ImGui/imgui_impl_win32.h"

#include "src/engine/input/windows/Input.h"

#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

namespace NextLevel
{
	/**
	* @brief 初期化処理をします。
	*/
	void Application::Init()
	{
		// ウィンドウの初期化
		m_window = WindowManager::CreateWindowInstanceForPlatform();
		m_window->Create(1280, 720, "NextLevel");

		// グラフィックスの初期化
		graphics::Graphics::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle(), m_window->GetWidth(), m_window->GetHeight());

		// エディタの初期化
		EditorManager::GetInstance().Startup(dynamic_cast<WindowsWindow*>
			(m_window.get())->GetWindowHandle());

		// シェーダーマネージャーのセーブデータ読み込み
		resource::ShaderManager::GetInstance().LoadFromJson();
		// マテリアルマネージャーのセーブデータ読み込み
		resource::MaterialManager::GetInstance().LoadFromJson();
		// モデルマネージャーのセーブデータ読み込み
		resource::ModelManager::GetInstance().LoadFromJson();

		// 入力の初期化
		InitInput();

		// 分解能
		timeBeginPeriod(1);
	}

	void Application::Uninit()
	{
		timeEndPeriod(1);

		// シェーダーマネージャーのセーブデータ読み込み
		resource::ShaderManager::GetInstance().SaveToJson();
		// マテリアルマネージャーのセーブデータ読み込み
		resource::MaterialManager::GetInstance().SaveToJson();
		// モデルマネージャーのセーブデータ読み込み
		resource::ModelManager::GetInstance().SaveToJson();

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
		// 入力の更新
		UpdateInput();
		
		graphics::Graphics::GetInstance().BeginRender(
			28.0f / 255.0f, 
			36.0f / 255.0f, 
			45.0f / 255.0f);
		EditorManager::GetInstance().BeginRender();

		ecs::WorldManager::GetInstance().Update(_deltaTime);

		EditorManager::GetInstance().Draw();

		ecs::WorldManager::GetInstance().Draw();

		EditorManager::GetInstance().EndRender();
		graphics::Graphics::GetInstance().EndRender();
	}

}