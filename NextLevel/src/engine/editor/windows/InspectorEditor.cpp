#include "InspectorEditor.h"

#include <bitset>

#include "ImGui/imgui.h"

#include "../Selectable.h"
#include "src/common/core/ecs/WorldManager.h"
#include "src/common/core/ecs/EntityManager.h"
#include "src/common/core/ecs/Utils.h"

namespace NextLevel
{
	/**
	* @brief エディタを描画します。
	*/
	void InspectorEditor::Draw()
	{
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

		SelectableInfo::EnSelect select = SelectableInfo::GetInstance().GetEnSelect();

		// 再選択チェック
		if (m_szSelectName != SelectableInfo::GetInstance().GetName())
		{
			// 名前を保存
			m_szSelectName = SelectableInfo::GetInstance().GetName();
			// 変更中断
			m_bWorldNameChanging = false;
			memset(m_szSetWorldName, '\0', 255);
			m_bEntityNameChanging = false;
			memset(m_szSetEntityName, '\0', 255);
		}

		// ワールドの場合
		if (select == SelectableInfo::enWorld)
		{
			auto world = ecs::WorldManager::GetInstance().GetWorld(SelectableInfo::GetInstance().GetName());

			// セレクト種類
			ImGui::Text("Select: Scene");

			// 開く
			ImGui::SameLine(ImGui::GetWindowWidth() - 80);
			if (ImGui::Button("Scene Open"))
			{
				// 選択ワールドを現在のワールドに設定
				ecs::WorldManager::GetInstance().SetCurrentWorld(SelectableInfo::GetInstance().GetName());
				// 変更中断
				m_bWorldNameChanging = false;
				memset(m_szSetWorldName, '\0', 255);
			}

			// ワールド名
			std::string text = "Name: " + SelectableInfo::GetInstance().GetName();
			if (m_bWorldNameChanging)
			{
				ImGui::InputText("Name", m_szSetWorldName, IM_ARRAYSIZE(m_szSetWorldName));
			}
			else
			{
				ImGui::Text(text.c_str());
			}

			// ワールド名変更
			if (m_bWorldNameChanging)
			{
				if (ImGui::Button("Set"))
				{
					if (m_szSetWorldName[0] != '\0')
					{
						// 名前を変更
						ecs::WorldManager::GetInstance().ChangeName(SelectableInfo::GetInstance().GetName(), m_szSetWorldName);
						SelectableInfo::GetInstance().SetName(m_szSetWorldName);
						// 変更終了
						m_bWorldNameChanging = false;
						memset(m_szSetWorldName, '\0', 255);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					m_bWorldNameChanging = false;
					// 初期化
					memset(m_szSetWorldName, '\0', 255);
				}
			}
			else
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 90);
				if (ImGui::Button("Change Name"))
				{
					m_bWorldNameChanging = true;
				}
			}

			// 水平線を挿入
			ImGui::Separator();

			// ドロップダウンを表示
			const char* systemList[] =
			{
				"TransformSystem",
				"MeshRendererSystem"
			};
			ImGui::Combo("System", &m_nSelectSystemIndex, systemList, IM_ARRAYSIZE(systemList));
			ImGui::SameLine(ImGui::GetWindowWidth() - 80);
			if (ImGui::Button("Add System"))
			{

			}

		}
		else
		{
			// 初期化
			memset(m_szSetWorldName, '\0', 255);
		}

		// エンティティの場合
		if (select == SelectableInfo::enEntity)
		{
			// セレクト種類
			ImGui::Text("Select: Entity");

			// エンティティ名
			std::string text = "Name: " + SelectableInfo::GetInstance().GetName();
			if (m_bEntityNameChanging)
			{
				ImGui::InputText("Name", m_szSetEntityName, IM_ARRAYSIZE(m_szSetEntityName));
			}
			else
			{
				ImGui::Text(text.c_str());
			}

			// ワールド名変更
			if (m_bEntityNameChanging)
			{
				if (ImGui::Button("Set"))
				{
					if (m_szSetEntityName[0] != '\0')
					{
						// 名前を変更
						ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->GetEntityManager()->
							ChangeName(SelectableInfo::GetInstance().GetBit(), m_szSetEntityName);
						SelectableInfo::GetInstance().SetName(m_szSetEntityName);
						// 変更終了
						m_bEntityNameChanging = false;
						memset(m_szSetEntityName, '\0', 255);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					m_bEntityNameChanging = false;
					// 初期化
					memset(m_szSetEntityName, '\0', 255);
				}
			}
			else
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 90);
				if (ImGui::Button("Change Name"))
				{
					m_bEntityNameChanging = true;
				}
			}

			// 水平線を挿入
			ImGui::Separator();

			// ドロップダウンを表示
			const char* systemList[] =
			{
				"Transform",
				"MeshRenderer"
			};
			ImGui::Combo("Component", &m_nSelectComponentIndex, systemList, IM_ARRAYSIZE(systemList));
			if (ImGui::Button("Add Component"))
			{
				//ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->GetEntityManager()->AddComponent<>
			}

			// アーキタイプを取得
			std::bitset<ecs::utils::cMaxComponentSize> signature = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
				GetEntityManager()->GetArchetype(SelectableInfo::GetInstance().GetBit()).GetSignature();

			// 全てのコンポーネントを反復
			for (int i = 0; i < ecs::utils::cMaxComponentSize; i++)
			{
				// 持っていなければ飛ばす
				if (!signature.test(i)) continue;


			}
		}
		else
		{
			// 初期化
			memset(m_szSetWorldName, '\0', 255);
		}

		ImGui::End();
	}
}