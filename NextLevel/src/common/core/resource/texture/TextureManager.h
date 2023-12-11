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
		* @brief �e�N�X�`�����Ǘ�����N���X�A
		*/
		class TextureManager
		{
		private:
			/**
			* @brief �R���X�g���N�^�B
			*/
			TextureManager() = default;

			/**
			* @brief �f�X�g���N�^�B
			*/
			~TextureManager() = default;

		public:
			/**
			 * @brief �V���O���g���C���X�^���X���擾���܂��B
			 * @return TextureManager& �e�N�X�`���}�l�[�W���[�N���X�̃C���X�^���X�B
			 */
			static TextureManager& GetInstance()
			{
				static TextureManager instance;
				return instance;
			}

		public:
			/**
			 * @brief �e�N�X�`�������[�h���A���̃C���X�^���X��Ԃ��܂��B
			 * @param _fileName �e�N�X�`���t�@�C���̖��O�B
			 * @return std::shared_ptr<TextureInstance> �e�N�X�`���C���X�^���X�B
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

				// �����̒T��
				auto it = m_idList.find(_fileName.c_str());
				if (it != m_idList.end())
					return it->second;

				// �V�K
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
			 * @brief �w�肳�ꂽID�̃e�N�X�`���f�[�^���擾���܂��B
			 * @param _textureId �e�N�X�`��ID�B
			 * @return std::shared_ptr<TextureData> �e�N�X�`���f�[�^�B
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
			//! �~���[�e�b�N�X
			std::mutex m_mutex;
			//! ���Ɋ��蓖�Ă�e�N�X�`��ID�B
			int m_nextId = 0;
			//! �t�@�C�����ƃe�N�X�`��ID�̃}�b�v�B
			std::unordered_map<const char*, int> m_idList;
			//! �e�N�X�`��ID�ƃe�N�X�`���f�[�^�̃}�b�v�B
			std::map<int, std::shared_ptr<TextureData>> m_textureMap;
		};
	}
}
#endif //!___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREMANAGER_H___