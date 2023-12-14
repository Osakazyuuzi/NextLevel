#include "ModelManager.h"

#include <iostream>
#include <fstream>

#include "Json/json.hpp"
#include "ModelData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief Json�t�@�C���Ƀf�[�^��ۑ����܂��B
		*/
		void ModelManager::SaveToJson()
		{
			nlohmann::json jsonFile;

			for (const auto& pair : m_idMap) {
				// ���f���t�@�C����
				jsonFile[pair.first] = pair.second;
				//// �A�j���[�V�����t�@�C����
				//const ModelData& model = *(m_modelMap[pair.second].get());
				//for (const auto& animPair : model) {
				//	j[pair.first]["animInfo"][animPair.first] = animPair.second;
				//}
			}

			std::ofstream saveFile(SAVE_FILENAME);
			saveFile << jsonFile.dump(4);
		}

		/**
		* @brief Json�t�@�C������f�[�^��ǂݍ��݂܂��B
		*/
		void ModelManager::LoadFromJson()
		{
			// �ۑ��t�@�C�����J��
			std::ifstream saveFile(SAVE_FILENAME);
			nlohmann::json jsonFile;
			saveFile >> jsonFile;

			int maxId = 0;
			for (auto it = jsonFile.begin(); it != jsonFile.end(); ++it) {
				std::shared_ptr<ModelData> modelData =
					std::make_shared<ModelData>();
				modelData->Load(it.key().c_str());
				int id = it.value().get<int>();
				m_idMap[it.key()] = id;
				m_modelMap[id] = modelData;
				maxId = maxId > id ? maxId : id;	// ID���X�V����

				//if (it.value().contains("animInfo")) {
				//	for (const auto& animPair : it.value()["animInfo"].items()) {
				//		data.m_animInfo[animPair.key().c_str()] = animPair.value().get<std::string>();
				//		data.LoadAnimation(animPair.value().get<std::string>().c_str(), animPair.key().c_str());
				//	}
				//}
			}
			// ID���X�V����
			m_nNextId = maxId + 1;
		}
	}
}