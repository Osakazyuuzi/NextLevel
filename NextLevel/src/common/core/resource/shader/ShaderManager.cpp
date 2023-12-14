#include "ShaderManager.h"

#include <iostream>
#include <fstream>

#include "Json/json.hpp"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief Jsonファイルにデータを保存します。
		*/
		void ShaderManager::SaveToJson()
		{
			nlohmann::json jsonFile;

			// 登録されているシェーダー毎に描き込みを行う
			for (const auto& pair : m_idMap) {
				nlohmann::json shaderJson;
				// シェーダーの種類を描き込み
				shaderJson["Kind"] = (int)GetShaderData(pair.second).value()->GetKind();
				// シェーダーのIDを描き込み
				shaderJson["ID"] = pair.second;
				// 作成したデータを追加
				jsonFile[pair.first] = shaderJson;
			}

			std::ofstream saveFile(SAVE_FILENAME);
			saveFile << jsonFile.dump(4);
		}

		/**
		* @brief Jsonファイルからデータを読み込みます。
		*/
		void ShaderManager::LoadFromJson()
		{
			// 保存ファイルを開く
			std::ifstream saveFile(SAVE_FILENAME);
			nlohmann::json jsonFile;
			saveFile >> jsonFile;

			int maxId = 0;
			// 登録されているシェーダーを全て反復する
			for (auto it = jsonFile.begin(); it != jsonFile.end(); ++it) {
				std::shared_ptr<ShaderData> shaderData = std::make_shared<ShaderData>();
				
				// シェーダーを読み込む
				shaderData->Load((ShaderData::EnKind)it.value()["Kind"].get<int>(), it.key().c_str());
				// IDを取得する
				int id = it.value()["ID"].get<int>();

				// それぞれマップに登録する
				m_idMap[it.key()] = id;
				m_shaderMap[id] = shaderData;

				// IDを更新する
				maxId = maxId > id ? maxId : id;
			}
			// NextIDを更新する
			m_nNextId = maxId + 1;
		}
	}
}