#include "HierarchyEditor.h"

#include <string>

#include "ImGui/imgui.h"
#include "../Selectable.h"

#include "src/common/core/ecs/World.h"
#include "src/common/core/ecs/WorldManager.h"
#include "src/common/core/ecs/EntityManager.h"

namespace NextLevel
{
	void HierarchyEditor::Draw()
	{
		ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoCollapse);

		// ワールド選択中のみ
		if (ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().size())
		{
			// ワールドヒエラルキーの最後尾を取得
			std::shared_ptr<ecs::World> currentWorld = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back();
			auto entities = currentWorld->GetEntityManager()->GetEntities();

			// ワールド名を表示
			std::string name = "Scene: ";
			name += currentWorld->GetName();
			ImGui::Text(name.c_str());

			// エンティティ作成ボタンを表示
			if (ImGui::Button("Create Entity"))
			{
				// エンティティを作成
				auto entity = currentWorld->GetEntityManager()->CreateEntity();

				// 作成したエンティティを選択する
				SelectableInfo::GetInstance().SetEnSelect(SelectableInfo::enEntity);
				SelectableInfo::GetInstance().SetBit(entity.m_Identifier);
			}

			// エンティティ列挙
			for (auto entity : entities)
			{
				if (ImGui::Selectable(entity.first.c_str(),
					SelectableInfo::GetInstance().GetEnSelect() == SelectableInfo::enEntity &&
					SelectableInfo::GetInstance().GetBit() == entity.second.second.m_Identifier
					))
				{
					// 選択したエンティティを設定する
					SelectableInfo::GetInstance().SetEnSelect(SelectableInfo::enEntity);
					SelectableInfo::GetInstance().SetName(entity.first);
					SelectableInfo::GetInstance().SetBit(entity.second.second.m_Identifier);
				}
			}
		}

		ImGui::End();
	}
}