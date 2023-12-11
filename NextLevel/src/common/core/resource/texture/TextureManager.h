#ifndef ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___
#define ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___

#include <memory>
#include <map>
#include <unordered_map>
#include <mutex>
#include <optional>

#include "TextureData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class TextureManager
		* @brief テクスチャを管理するクラス、
		*/
		class TextureManager
		{
		private:
			/**
			* @brief コンストラクタ。
			*/
			TextureManager() = default;

			/**
			* @brief デストラクタ。
			*/
			~TextureManager() = default;

		public:
			/**
			 * @brief シングルトンインスタンスを取得します。
			 * @return TextureManager& テクスチャマネージャークラスのインスタンス。
			 */
			static TextureManager& GetInstance()
			{
				static TextureManager instance;
				return instance;
			}

		public:
			/**
			 * @brief テクスチャをロードし、そのインスタンスを返します。
			 * @param _fileName テクスチャファイルの名前。
			 * @return std::shared_ptr<TextureInstance> テクスチャインスタンス。
			 */
			std::shared_ptr<TextureData> LoadTexture(const char* _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				auto it = m_idList.find(_fileName);
				if (it != m_idList.end())
					return m_textureMap[it->second];

				int newId = m_nextId++;
				m_idList[_fileName] = newId;
				m_textureMap[newId] = std::make_shared<TextureData>();

				return m_textureMap[newId];
			}

			std::optional<int> GetTextureID(std::string _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// 既存の探索
				auto it = m_idList.find(_fileName.c_str());
				if (it != m_idList.end())
					return it->second;

				// 新規
				std::shared_ptr<TextureData> newTexture
					= std::make_shared<TextureData>();
				if (FAILED(newTexture->Create(_fileName.c_str())))
					return {};

				int newId = m_nextId;
				m_idList[_fileName.c_str()] = newId;
				m_textureMap[newId] = newTexture;
				m_nextId++;

				return newId;
			}

			/**
			 * @brief 指定されたIDのテクスチャデータを取得します。
			 * @param _textureId テクスチャID。
			 * @return std::shared_ptr<TextureData> テクスチャデータ。
			 */
			std::shared_ptr<TextureData> GetTextureData(int _textureId)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				auto it = m_textureMap.find(_textureId);
				if (it != m_textureMap.end())
					return it->second;
				return nullptr;
			}

		private:
			//! ミューテックス
			std::mutex m_mutex;
			//! 次に割り当てるテクスチャID。
			int m_nextId = 0;
			//! ファイル名とテクスチャIDのマップ。
			std::unordered_map<const char*, int> m_idList;
			//! テクスチャIDとテクスチャデータのマップ。
			std::map<int, std::shared_ptr<TextureData>> m_textureMap;
		};
	}
}
#endif //!___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___