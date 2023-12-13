#include "SceneGraphEditor.h"

#include "third_party/ImGui/imgui.h"

#include "src/common/core/ecs/WorldManager.h"
#include "../Selectable.h"

namespace NextLevel
{
	/**
	* @brief �G�f�B�^��`�悵�܂��B
	*/
	void SceneGraphEditor::Draw()
	{
		ImGui::Begin("SceneGraph", nullptr, ImGuiWindowFlags_NoCollapse);
		
		// ���[���h�쐬
		if (ImGui::Button("Create New Scene"))
		{
			ecs::WorldManager::GetInstance().CreateWorld();
		}

		// ���[���h�\��
		auto& worldMap = ecs::WorldManager::GetInstance().GetWorldMap();
		auto& selectable = SelectableInfo::GetInstance();
		for (auto&& it : worldMap)
		{
			bool selected = selectable.GetEnSelect() == SelectableInfo::enWorld &&
				(it.first == selectable.GetName());

			if (ImGui::Selectable(it.first.c_str(), 
				// ���[���h��I�𒆂����O���������ꍇ����
				selected))
			{
				// �I�𒆂ɐݒ�
				selectable.SetEnSelect(SelectableInfo::enWorld);
				selectable.SetName(it.first);
			}
		}

		ImGui::End();
	}
}