#include "InspectorEditor.h"

#include <bitset>

#include "ImGui/imgui.h"

#include "../Selectable.h"
#include "src/common/core/ecs/WorldManager.h"
#include "src/common/core/ecs/EntityManager.h"
#include "src/common/core/ecs/Utils.h"
#include "src/common/core/utils/TypeInfo.h"

#include "src/common/core/ecs/component/CCamera.h"
#include "src/common/core/ecs/component/CTransform.h"
#include "src/common/core/ecs/component/CMeshRenderer.h"
#include "src/common/core/ecs/component/CDebugCamera.h"

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

				// コンポーネントの表示
				if (i == TypeManager::TypeInfo<ecs::component::CTransform>::GetID())
				{
					// コンポーネントの取得
					auto comp = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
						GetEntityManager()->GetComponent<ecs::component::CTransform>(SelectableInfo::GetInstance().GetBit());

					if (ImGui::TreeNode("Transform")) {
						
						ImGui::DragFloat3("Position", &comp->m_position.x()); 
						ImGui::DragFloat3("Rotation", &comp->m_rotation.x());
						ImGui::DragFloat3("Scale", &comp->m_scale.x());
						
						ImGui::TreePop();
					}
				}
				else if (i == TypeManager::TypeInfo<ecs::component::CCamera>::GetID())
				{
					// コンポーネントの取得
					auto comp = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
						GetEntityManager()->GetComponent<ecs::component::CCamera>(SelectableInfo::GetInstance().GetBit());

					if (ImGui::TreeNode("Camera")) {

						ImGui::DragFloat("NearClip", &comp->m_fNearClip);
						ImGui::DragFloat("FarClip", &comp->m_fFarClip);
						ImGui::DragFloat("Aspect", &comp->m_fAspect);
						ImGui::DragFloat("Fovy", &comp->m_fFovy);

						ImGui::TreePop();
					}
				}
				else if (i == TypeManager::TypeInfo<ecs::component::CDebugCamera>::GetID())
				{
					// コンポーネントの取得
					auto comp = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
						GetEntityManager()->GetComponent<ecs::component::CDebugCamera>(SelectableInfo::GetInstance().GetBit());

					if (ImGui::TreeNode("DebugCamera")) {

						ImGui::DragFloat3("Position", &comp->m_Pos.x);
						ImGui::DragFloat3("LookPos", &comp->m_Look.x);
						ImGui::DragFloat3("UpVec", &comp->m_Up.x);
						ImGui::DragFloat("NearClip", &comp->m_fNearClip);
						ImGui::DragFloat("FarClip", &comp->m_fFarClip);
						ImGui::DragFloat("Aspect", &comp->m_fAspect);
						ImGui::DragFloat("Fovy", &comp->m_fFovy);

						ImGui::TreePop();
					}
				}
				else if (i == TypeManager::TypeInfo<ecs::component::CMeshRenderer>::GetID())
				{
					// コンポーネントの取得
					auto comp = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
						GetEntityManager()->GetComponent<ecs::component::CMeshRenderer>(SelectableInfo::GetInstance().GetBit());

					if (ImGui::TreeNode("MeshRenderer")) {

						ImGui::InputInt("ModelID", &comp->m_modelId);
						ImGui::InputInt("MaterialID", &comp->m_materialId);
						ImGui::InputInt("VertexShaderID", &comp->m_vertexShaderId);
						ImGui::InputInt("PixelShaderID", &comp->m_pixelShaderId);

						ImGui::TreePop();
					}
				}
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