#include "EditorManager.h"

#include "third_party/ImGui/imgui.h"
#include "third_party/ImGui/imconfig.h"
#include "third_party/ImGui/imgui_impl_dx11.h"
#include "third_party/ImGui/imgui_impl_win32.h"

#include "src/engine/graphics/windows/Graphics.h"

#include "src/common/core/ecs/WorldManager.h"

namespace NextLevel
{
	/**
	* @brief エディタの初期化をします。
	* @param _hWnd ウィンドウハンドル。
	*/
	void EditorManager::Startup(HWND _hWnd)
	{
		ecs::WorldManager::GetInstance().CreateWorld();
		ecs::WorldManager::GetInstance().CreateWorld();
		ecs::WorldManager::GetInstance().CreateWorld();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			//style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// ボタンの背景色
		style.Colors[ImGuiCol_Button]			= ImVec4({ 170.0f / 255.0f, 210.0f / 255.0f, 190.0f / 255.0f, 1.0f });
		style.Colors[ImGuiCol_ButtonHovered]	= ImVec4({ 190.0f / 255.0f, 230.0f / 255.0f, 210.0f / 255.0f, 1.0f });
		style.Colors[ImGuiCol_ButtonActive]		= ImVec4({ 210.0f / 255.0f, 250.0f / 255.0f, 230.0f / 255.0f, 1.0f });

		// 文字色
		style.Colors[ImGuiCol_Text] = ImVec4(70.0f / 255.0f, 90.0f / 255.0f, 80.0f / 255.0f, 1.0f);

		// 選択中の色
		style.Colors[ImGuiCol_Header] = ImVec4(190.0f / 255.0f, 230.0f / 255.0f, 210.0f / 255.0f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(210.0f / 255.0f, 250.0f / 255.0f, 230.0f / 255.0f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(215.0f / 255.0f, 255.0f / 255.0f, 235.0f / 255.0f, 1.0f);

		ImGui_ImplWin32_Init(_hWnd);
		ImGui_ImplDX11_Init(
			graphics::Graphics::GetInstance().GetDevice().Get(),
			graphics::Graphics::GetInstance().GetDeviceContext().Get()
			);
	}

	/**
	* @brief エディタの終了処理をします。
	*/
	void EditorManager::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	/**
	* @brief エディタの描画前準備をします。
	*/
	void EditorManager::BeginRender()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	/**
	* @brief エディタの描画後処理をします。
	*/
	void EditorManager::EndRender()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	/**
	* @brief エディタの描画をします。
	*/
	void EditorManager::Draw()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		// 色
		ImVec4 color = ImVec4(120.0f / 255.0f, 180.0f / 255.0f, 160.0f / 255.0f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = color;
		style.Colors[ImGuiCol_TitleBg] = color;
		style.Colors[ImGuiCol_TitleBgActive] = color;
		style.Colors[ImGuiCol_TitleBgCollapsed] = color;

		// メニューバー
		io.FontGlobalScale = 1.0f;
		ImGui::SetNextWindowSize({1280, 35});
		ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::Button("Master")) m_bMaster ^= true;
		ImGui::SameLine();
		if (ImGui::Button("SceneGraph")) m_bSceneGraph ^= true;
		ImGui::SameLine();
		if (ImGui::Button("Hierarchy")) m_bHierarchy ^= true;
		ImGui::SameLine();
		if (ImGui::Button("Inspector")) m_bInspector ^= true;
		ImGui::SameLine();
		if (ImGui::Button("Assets")) m_bAssets ^= true;
		ImGui::SameLine();
		if (ImGui::Button("Console")) m_bConsole ^= true;
		ImGui::SameLine();
		if (ImGui::Button("Debug")) m_bDebug ^= true;
		ImGui::SameLine();
		ImGui::End();
		
		// ボタン色を変更
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(120.0f / 255.0f, 180.0f / 255.0f, 160.0f / 255.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(130.0f / 255.0f, 190.0f / 255.0f, 170.0f / 255.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(140.0f / 255.0f, 200.0f / 255.0f, 180.0f / 255.0f, 1.0f));
		// ウィンドウの背景色を変更
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(150.0f / 255.0f, 200.0f / 255.0f, 180.0f / 255.0f, 1.0f));

		// マスター
		if (m_bMaster)
		{
			ImGui::Begin("Master", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// シーングラフ
		if (m_bSceneGraph) m_SceneGraphEditor.Draw();
		// ヒエラルキー
		if (m_bHierarchy) m_HierarchyEditor.Draw();
		// インスペクター
		if (m_bInspector) m_InspectorEditor.Draw();

		// アセット
		if (m_bAssets)
		{
			ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// コンソール
		if (m_bConsole)
		{
			ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// デバッグ
		if (m_bDebug)
		{
			ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::Text("ElapsedTime: %.2f", m_timer.GetElapsedTime());
			ImGui::Text("DeltaTime: %.4f", m_timer.GetDeltaTime());
			ImGui::Text("FPS: %.2f", 1.0f / m_timer.GetDeltaTime());
			m_timer.Reset();

			ImGui::End();
		}

		ImGui::PopStyleColor(4);
	}
}