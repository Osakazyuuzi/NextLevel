#ifndef ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___
#define ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___

#include <memory>
#include <map>
#include <unordered_map>

#include "TextureData.h"
#include "TextureInstance.h"

namespace NextLevel
{
	namespace resource
	{
		/**
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
			std::shared_ptr<TextureInstance> LoadTexture(const char* _fileName)
			{
				auto it = m_idList.find(_fileName);
				if (it != m_idList.end())
					return std::make_shared<TextureInstance>(it->second);

				int newId = m_nextId++;
				m_idList[_fileName] = newId;
				m_textureMap[newId] = std::make_shared<TextureData>(_fileName);

				return std::make_shared<TextureInstance>(newId);
			}

			/**
			 * @brief 指定されたIDのテクスチャデータを取得します。
			 * @param _textureId テクスチャID。
			 * @return std::shared_ptr<TextureData> テクスチャデータ。
			 */
			std::shared_ptr<TextureData> GetTextureData(int _textureId)
			{
				auto it = m_textureMap.find(_textureId);
				if (it != m_textureMap.end())
					return it->second;
				return nullptr;
			}

		private:
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