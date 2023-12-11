#ifndef ___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALMANAGER_H___
#define ___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALMANAGER_H___

#include <memory>
#include <iostream>
#include <mutex>
#include <map>
#include <optional>

#include "MaterialData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class MaterialManager
		* @brief マテリアルを管理するクラス。
		*/
		class MaterialManager {
		public:
			/**
			* @brief マテリアルを新規作成します。
			* @return int マテリアルに紐づくID。
			*/
			int CreateMaterial()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 新たに作成
				int newId = m_nNextId++;
				m_materialMap[newId] = std::make_shared<MaterialData>();

				return newId;
			}

			/**
			* @brief マテリアルIDを取得します。
			* @param _id マテリアルID。
			* @return std::optional<std::shared_ptr<MaterialData>> マテリアルデータ。
			*/
			std::optional<std::shared_ptr<MaterialData>> GetMaterialData(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 既存リストから探索
				auto it = m_materialMap.find(_id);
				if (it != m_materialMap.end()) return it->second;

				return {};
			}

			/**
			* @brief マテリアルマップからマテリアルを削除します。
			* @param _id マテリアルID。
			*/
			void DestroyMaterial(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 既存リストから探索
				auto it = m_materialMap.find(_id);
				if (it != m_materialMap.end()) m_materialMap.erase(it);
			}

			/**
			* @brief 全てのマテリアルを取得します。
			* @return std::map<int, std::shared_ptr<MaterialData>>& マテリアルマップ。
			*/
			std::map<int, std::shared_ptr<MaterialData>>& GetMaterialDatas()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				return m_materialMap;
			}

		private:
			//! ミューテックス
			std::mutex m_mutex;
			//! 次に割り当てるID。
			int m_nNextId = 0;
			//! IDとマテリアルを紐づけるマップ
			std::map<int, std::shared_ptr<MaterialData>> m_materialMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALMANAGER_H___