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
	* @brief �G�f�B�^��`�悵�܂��B
	*/
	void InspectorEditor::Draw()
	{
		ImGui::Begin("Inspector", nullptr, ImGuiWindowFlags_NoCollapse);

		SelectableInfo::EnSelect select = SelectableInfo::GetInstance().GetEnSelect();

		// �đI���`�F�b�N
		if (m_szSelectName != SelectableInfo::GetInstance().GetName())
		{
			// ���O��ۑ�
			m_szSelectName = SelectableInfo::GetInstance().GetName();
			// �ύX���f
			m_bWorldNameChanging = false;
			memset(m_szSetWorldName, '\0', 255);
			m_bEntityNameChanging = false;
			memset(m_szSetEntityName, '\0', 255);
		}

		// ���[���h�̏ꍇ
		if (select == SelectableInfo::enWorld)
		{
			auto world = ecs::WorldManager::GetInstance().GetWorld(SelectableInfo::GetInstance().GetName());

			// �Z���N�g���
			ImGui::Text("Select: Scene");

			// �J��
			ImGui::SameLine(ImGui::GetWindowWidth() - 80);
			if (ImGui::Button("Scene Open"))
			{
				// �I�����[���h�����݂̃��[���h�ɐݒ�
				ecs::WorldManager::GetInstance().SetCurrentWorld(SelectableInfo::GetInstance().GetName());
				// �ύX���f
				m_bWorldNameChanging = false;
				memset(m_szSetWorldName, '\0', 255);
			}

			// ���[���h��
			std::string text = "Name: " + SelectableInfo::GetInstance().GetName();
			if (m_bWorldNameChanging)
			{
				ImGui::InputText("Name", m_szSetWorldName, IM_ARRAYSIZE(m_szSetWorldName));
			}
			else
			{
				ImGui::Text(text.c_str());
			}

			// ���[���h���ύX
			if (m_bWorldNameChanging)
			{
				if (ImGui::Button("Set"))
				{
					if (m_szSetWorldName[0] != '\0')
					{
						// ���O��ύX
						ecs::WorldManager::GetInstance().ChangeName(SelectableInfo::GetInstance().GetName(), m_szSetWorldName);
						SelectableInfo::GetInstance().SetName(m_szSetWorldName);
						// �ύX�I��
						m_bWorldNameChanging = false;
						memset(m_szSetWorldName, '\0', 255);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					m_bWorldNameChanging = false;
					// ������
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

			// ��������}��
			ImGui::Separator();

			// �h���b�v�_�E����\��
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
			// ������
			memset(m_szSetWorldName, '\0', 255);
		}

		// �G���e�B�e�B�̏ꍇ
		if (select == SelectableInfo::enEntity)
		{
			// �Z���N�g���
			ImGui::Text("Select: Entity");

			// �G���e�B�e�B��
			std::string text = "Name: " + SelectableInfo::GetInstance().GetName();
			if (m_bEntityNameChanging)
			{
				ImGui::InputText("Name", m_szSetEntityName, IM_ARRAYSIZE(m_szSetEntityName));
			}
			else
			{
				ImGui::Text(text.c_str());
			}

			// ���[���h���ύX
			if (m_bEntityNameChanging)
			{
				if (ImGui::Button("Set"))
				{
					if (m_szSetEntityName[0] != '\0')
					{
						// ���O��ύX
						ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->GetEntityManager()->
							ChangeName(SelectableInfo::GetInstance().GetBit(), m_szSetEntityName);
						SelectableInfo::GetInstance().SetName(m_szSetEntityName);
						// �ύX�I��
						m_bEntityNameChanging = false;
						memset(m_szSetEntityName, '\0', 255);
					}
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					m_bEntityNameChanging = false;
					// ������
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

			// ��������}��
			ImGui::Separator();

			// �h���b�v�_�E����\��
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

			// �A�[�L�^�C�v���擾
			std::bitset<ecs::utils::cMaxComponentSize> signature = ecs::WorldManager::GetInstance().GetCurrentWorldHierarchy().back()->
				GetEntityManager()->GetArchetype(SelectableInfo::GetInstance().GetBit()).GetSignature();

			// �S�ẴR���|�[�l���g�𔽕�
			for (int i = 0; i < ecs::utils::cMaxComponentSize; i++)
			{
				// �����Ă��Ȃ���Δ�΂�
				if (!signature.test(i)) continue;


			}
		}
		else
		{
			// ������
			memset(m_szSetWorldName, '\0', 255);
		}

		ImGui::End();
	}
}