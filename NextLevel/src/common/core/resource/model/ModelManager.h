#ifndef ___SRC_COMMON_CORE_RESOURCE_MODEL_MODELMANAGER_H___
#define ___SRC_COMMON_CORE_RESOURCE_MODEL_MODELMANAGER_H___

#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <optional>
#include <memory>

#include "ModelData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class ModelManager
		* @brief モデルを管理するクラス。
		*/
		class ModelManager {
		private:
			/**
			* @brief コンストラクタ。
			*/
			ModelManager() = default;

			/**
			* @brief デストラクタ。
			*/
			~ModelManager() = default;

		public:
			/**
			 * @brief シングルトンインスタンスを取得します。
			 * @return ModelManager& マネージャークラスのインスタンス。
			 */
			static ModelManager& GetInstance()
			{
				static ModelManager instance;
				return instance;
			}

		private:
			//! セーブファイル名
			static constexpr const char* SAVE_FILENAME = "assets/saveData/ModelData.json";

		public:
			/**
			* @brief Jsonファイルにデータを保存します。
			*/
			void SaveToJson();

			/**
			* @brief Jsonファイルからデータを読み込みます。
			*/
			void LoadFromJson();

		public:
			/**
			* @brief モデルIDを取得します。未読み込みであればロードします。
			* @param _fileName ファイル名。
			* @return std::optional<int> モデルに割り当てられたID。
			*/
			std::optional<int> GetAndLoadModelID(std::string _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 既存の探索
				auto it = m_idMap.find(_fileName);
				if (it != m_idMap.end()) return it->second;

				// 新規で読み込み
				std::shared_ptr<ModelData> newModel
					= std::make_shared<ModelData>();
				if (!newModel->Load(_fileName)) return {};

				// 登録
				int newId = m_nNextId;
				m_idMap[_fileName] = newId;
				m_modelMap[newId] = newModel;
				m_nNextId++;

				return newId;
			}

			/**
			* @brief モデルデータを取得します。
			* @param _modelId モデルのID。
			* @return std::optional<std::shared_ptr<ModelData>> モデルデータ。
			*/
			std::optional<std::shared_ptr<ModelData>> GetModelData(int _modelId)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 既存の探索
				auto it = m_modelMap.find(_modelId);
				if (it != m_modelMap.end()) return it->second;

				return {};
			}

		private:
			//! ミューテックス。
			std::mutex m_mutex;
			//! 次に割り当てるID。
			int m_nNextId = 0;
			//! ファイル名とIDを紐づけるマップ。
			std::unordered_map<std::string, int> m_idMap;
			//! IDとモデルデータを紐づけるマップ。
			std::map<int, std::shared_ptr<ModelData>> m_modelMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_MODEL_MODELMANAGER_H___