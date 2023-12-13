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

		// ���[���h�I�𒆂̂�
		if (ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().size())
		{
			// ���[���h�q�G�����L�[�̍Ō�����擾
			std::shared_ptr<ecs::World> currentWorld = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back();
			auto entities = currentWorld->GetEntityManager()->GetEntities();

			// ���[���h����\��
			std::string name = "Scene: ";
			name += currentWorld->GetName();
			ImGui::Text(name.c_str());

			// �G���e�B�e�B�쐬�{�^����\��
			if (ImGui::Button("Create Entity"))
			{
				// �G���e�B�e�B���쐬
				auto entity = currentWorld->GetEntityManager()->CreateEntity();

				// �쐬�����G���e�B�e�B��I������
				SelectableInfo::GetInstance().SetEnSelect(SelectableInfo::enEntity);
				SelectableInfo::GetInstance().SetBit(entity.m_Identifier);
			}

			// �G���e�B�e�B��
			for (auto entity : entities)
			{
				if (ImGui::Selectable(entity.first.c_str(),
					SelectableInfo::GetInstance().GetEnSelect() == SelectableInfo::enEntity &&
					SelectableInfo::GetInstance().GetBit() == entity.second.second.m_Identifier
					))
				{
					// �I�������G���e�B�e�B��ݒ肷��
					SelectableInfo::GetInstance().SetEnSelect(SelectableInfo::enEntity);
					SelectableInfo::GetInstance().SetName(entity.first);
					SelectableInfo::GetInstance().SetBit(entity.second.second.m_Identifier);
				}
			}
		}

		ImGui::End();
	}
}