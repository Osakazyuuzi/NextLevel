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
	* @brief �G�f�B�^�̏����������܂��B
	* @param _hWnd �E�B���h�E�n���h���B
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

		// �{�^���̔w�i�F
		style.Colors[ImGuiCol_Button]			= ImVec4({ 170.0f / 255.0f, 210.0f / 255.0f, 190.0f / 255.0f, 1.0f });
		style.Colors[ImGuiCol_ButtonHovered]	= ImVec4({ 190.0f / 255.0f, 230.0f / 255.0f, 210.0f / 255.0f, 1.0f });
		style.Colors[ImGuiCol_ButtonActive]		= ImVec4({ 210.0f / 255.0f, 250.0f / 255.0f, 230.0f / 255.0f, 1.0f });

		// �����F
		style.Colors[ImGuiCol_Text] = ImVec4(70.0f / 255.0f, 90.0f / 255.0f, 80.0f / 255.0f, 1.0f);

		// �I�𒆂̐F
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
	* @brief �G�f�B�^�̏I�����������܂��B
	*/
	void EditorManager::Shutdown()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
	}

	/**
	* @brief �G�f�B�^�̕`��O���������܂��B
	*/
	void EditorManager::BeginRender()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	/**
	* @brief �G�f�B�^�̕`��㏈�������܂��B
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
	* @brief �G�f�B�^�̕`������܂��B
	*/
	void EditorManager::Draw()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		// �F
		ImVec4 color = ImVec4(120.0f / 255.0f, 180.0f / 255.0f, 160.0f / 255.0f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = color;
		style.Colors[ImGuiCol_TitleBg] = color;
		style.Colors[ImGuiCol_TitleBgActive] = color;
		style.Colors[ImGuiCol_TitleBgCollapsed] = color;

		// ���j���[�o�[
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
		
		// �{�^���F��ύX
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(120.0f / 255.0f, 180.0f / 255.0f, 160.0f / 255.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(130.0f / 255.0f, 190.0f / 255.0f, 170.0f / 255.0f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(140.0f / 255.0f, 200.0f / 255.0f, 180.0f / 255.0f, 1.0f));
		// �E�B���h�E�̔w�i�F��ύX
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(150.0f / 255.0f, 200.0f / 255.0f, 180.0f / 255.0f, 1.0f));

		// �}�X�^�[
		if (m_bMaster)
		{
			ImGui::Begin("Master", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// �V�[���O���t
		if (m_bSceneGraph) m_SceneGraphEditor.Draw();
		// �q�G�����L�[
		if (m_bHierarchy) m_HierarchyEditor.Draw();
		// �C���X�y�N�^�[
		if (m_bInspector) m_InspectorEditor.Draw();

		// �A�Z�b�g
		if (m_bAssets)
		{
			ImGui::Begin("Assets", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// �R���\�[��
		if (m_bConsole)
		{
			ImGui::Begin("Console", nullptr, ImGuiWindowFlags_NoCollapse);

			ImGui::End();
		}

		// �f�o�b�O
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