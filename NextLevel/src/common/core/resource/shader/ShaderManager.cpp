#include "ShaderManager.h"

#include <iostream>
#include <fstream>

#include "Json/json.hpp"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief Json�t�@�C���Ƀf�[�^��ۑ����܂��B
		*/
		void ShaderManager::SaveToJson()
		{
			nlohmann::json jsonFile;

			// �o�^����Ă���V�F�[�_�[���ɕ`�����݂��s��
			for (const auto& pair : m_idMap) {
				nlohmann::json shaderJson;
				// �V�F�[�_�[�̎�ނ�`������
				shaderJson["Kind"] = (int)GetShaderData(pair.second).value()->GetKind();
				// �V�F�[�_�[��ID��`������
				shaderJson["ID"] = pair.second;
				// �쐬�����f�[�^��ǉ�
				jsonFile[pair.first] = shaderJson;
			}

			std::ofstream saveFile(SAVE_FILENAME);
			saveFile << jsonFile.dump(4);
		}

		/**
		* @brief Json�t�@�C������f�[�^��ǂݍ��݂܂��B
		*/
		void ShaderManager::LoadFromJson()
		{
			// �ۑ��t�@�C�����J��
			std::ifstream saveFile(SAVE_FILENAME);
			nlohmann::json jsonFile;
			saveFile >> jsonFile;

			int maxId = 0;
			// �o�^����Ă���V�F�[�_�[��S�Ĕ�������
			for (auto it = jsonFile.begin(); it != jsonFile.end(); ++it) {
				std::shared_ptr<ShaderData> shaderData = std::make_shared<ShaderData>();
				
				// �V�F�[�_�[��ǂݍ���
				shaderData->Load((ShaderData::EnKind)it.value()["Kind"].get<int>(), it.key().c_str());
				// ID���擾����
				int id = it.value()["ID"].get<int>();

				// ���ꂼ��}�b�v�ɓo�^����
				m_idMap[it.key()] = id;
				m_shaderMap[id] = shaderData;

				// ID���X�V����
				maxId = maxId > id ? maxId : id;
			}
			// NextID���X�V����
			m_nNextId = maxId + 1;
		}
	}
}