#include "SceneGraphEditor.h"

#include "third_party/ImGui/imgui.h"

#include "src/common/core/ecs/WorldManager.h"
#include "../Selectable.h"

namespace NextLevel
{
	/**
	* @brief エディタを描画します。
	*/
	void SceneGraphEditor::Draw()
	{
		ImGui::Begin("SceneGraph", nullptr, ImGuiWindowFlags_NoCollapse);
		
		// ワールド作成
		if (ImGui::Button("Create New Scene"))
		{
			ecs::WorldManager::GetInstance().CreateWorld();
		}

		// ワールド表示
		auto& worldMap = ecs::WorldManager::GetInstance().GetWorldMap();
		auto& selectable = SelectableInfo::GetInstance();
		for (auto&& it : worldMap)
		{
			bool selected = selectable.GetEnSelect() == SelectableInfo::enWorld &&
				(it.first == selectable.GetName());

			if (ImGui::Selectable(it.first.c_str(), 
				// ワールドを選択中かつ名前が等しい場合強調
				selected))
			{
				// 選択中に設定
				selectable.SetEnSelect(SelectableInfo::enWorld);
				selectable.SetName(it.first);
			}
		}

		ImGui::End();
	}
}