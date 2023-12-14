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
		* @brief Jsonファイルにデータを保存します。
		*/
		void ModelManager::SaveToJson()
		{
			nlohmann::json jsonFile;

			for (const auto& pair : m_idMap) {
				// モデルファイル名
				jsonFile[pair.first] = pair.second;
				//// アニメーションファイル名
				//const ModelData& model = *(m_modelMap[pair.second].get());
				//for (const auto& animPair : model) {
				//	j[pair.first]["animInfo"][animPair.first] = animPair.second;
				//}
			}

			std::ofstream saveFile(SAVE_FILENAME);
			saveFile << jsonFile.dump(4);
		}

		/**
		* @brief Jsonファイルからデータを読み込みます。
		*/
		void ModelManager::LoadFromJson()
		{
			// 保存ファイルを開く
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
				maxId = maxId > id ? maxId : id;	// IDを更新する

				//if (it.value().contains("animInfo")) {
				//	for (const auto& animPair : it.value()["animInfo"].items()) {
				//		data.m_animInfo[animPair.key().c_str()] = animPair.value().get<std::string>();
				//		data.LoadAnimation(animPair.value().get<std::string>().c_str(), animPair.key().c_str());
				//	}
				//}
			}
			// IDを更新する
			m_nNextId = maxId + 1;
		}
	}
}