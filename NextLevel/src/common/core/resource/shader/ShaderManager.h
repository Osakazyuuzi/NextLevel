#ifndef ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERMANAGER_H___
#define ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERMANAGER_H___

#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <mutex>
#include <optional>
#include <memory>

#include "ShaderData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class ShaderManager
		* @brief シェーダーを管理するクラス。
		*/
		class ShaderManager {
		private:
			/**
			* @brief コンストラクタ。
			*/
			ShaderManager() = default;

			/**
			* @brief デストラクタ。
			*/
			~ShaderManager() = default;

		public:
			/**
			 * @brief シングルトンインスタンスを取得します。
			 * @return ShaderManager& シェーダーマネージャークラスのインスタンス。
			 */
			static ShaderManager& GetInstance()
			{
				static ShaderManager instance;
				return instance;
			}

		public:
			/**
			* @brief シェーダーIDを取得します。。未読み込みであればロードします。
			* @param _kind シェーダーの種類。
			* @param _fileName シェーダーのファイル名。
			* @return int シェーダーに割り当てられたID。
			*/
			std::optional<int> GetAndLoadShaderID(ShaderData::EnKind _kind, std::string _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 未読み込みかを探索
				auto it = m_idMap.find(_fileName);
				if (it != m_idMap.end()) return it->second;

				// 新たに読み込み
				std::shared_ptr<ShaderData> newShader =
					std::make_shared<ShaderData>();
				if (!newShader->Load(_kind, _fileName.c_str())) return {};

				// 割り当て
				int newId = m_nNextId;
				m_idMap[_fileName] = newId;
				m_shaderMap[newId] = newShader;
				m_nNextId++;

				return newId;
			}

			/**
			* @brief シェーダーデータを取得します。
			* @param _id シェーダーのID。
			* @return シェーダーデータ。
			*/
			std::optional<std::shared_ptr<ShaderData>> GetShaderData(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 未読み込みかを探索
				auto it = m_shaderMap.find(_id);
				if (it != m_shaderMap.end()) return it->second;

				return {};
			}

			/**
			* @brief 全てのシェーダーを取得します。
			* @return std::map<int, std::shared_ptr<ShaderData>>& シェーダーマップ。
			*/
			const std::map<int, std::shared_ptr<ShaderData>>& GetShaderDatas()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				return m_shaderMap;
			}

		private:
			//! ミューテックス。
			std::mutex m_mutex;
			//! 次に割り当てるID。
			int m_nNextId = 0;
			//! ファイル名とIDを紐づけるマップ。
			std::unordered_map<std::string, int> m_idMap;
			//! IDとシェーダーデータを紐づけるマップ。
			std::map<int, std::shared_ptr<ShaderData>> m_shaderMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERMANAGER_H___