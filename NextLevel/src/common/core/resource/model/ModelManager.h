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
		* @brief ���f�����Ǘ�����N���X�B
		*/
		class ModelManager {
		private:
			/**
			* @brief �R���X�g���N�^�B
			*/
			ModelManager() = default;

			/**
			* @brief �f�X�g���N�^�B
			*/
			~ModelManager() = default;

		public:
			/**
			 * @brief �V���O���g���C���X�^���X���擾���܂��B
			 * @return ModelManager& �}�l�[�W���[�N���X�̃C���X�^���X�B
			 */
			static ModelManager& GetInstance()
			{
				static ModelManager instance;
				return instance;
			}

		private:
			//! �Z�[�u�t�@�C����
			static constexpr const char* SAVE_FILENAME = "assets/saveData/ModelData.json";

		public:
			/**
			* @brief Json�t�@�C���Ƀf�[�^��ۑ����܂��B
			*/
			void SaveToJson();

			/**
			* @brief Json�t�@�C������f�[�^��ǂݍ��݂܂��B
			*/
			void LoadFromJson();

		public:
			/**
			* @brief ���f��ID���擾���܂��B���ǂݍ��݂ł���΃��[�h���܂��B
			* @param _fileName �t�@�C�����B
			* @return std::optional<int> ���f���Ɋ��蓖�Ă�ꂽID�B
			*/
			std::optional<int> GetAndLoadModelID(std::string _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// �����̒T��
				auto it = m_idMap.find(_fileName);
				if (it != m_idMap.end()) return it->second;

				// �V�K�œǂݍ���
				std::shared_ptr<ModelData> newModel
					= std::make_shared<ModelData>();
				if (!newModel->Load(_fileName)) return {};

				// �o�^
				int newId = m_nNextId;
				m_idMap[_fileName] = newId;
				m_modelMap[newId] = newModel;
				m_nNextId++;

				return newId;
			}

			/**
			* @brief ���f���f�[�^���擾���܂��B
			* @param _modelId ���f����ID�B
			* @return std::optional<std::shared_ptr<ModelData>> ���f���f�[�^�B
			*/
			std::optional<std::shared_ptr<ModelData>> GetModelData(int _modelId)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// �����̒T��
				auto it = m_modelMap.find(_modelId);
				if (it != m_modelMap.end()) return it->second;

				return {};
			}

		private:
			//! �~���[�e�b�N�X�B
			std::mutex m_mutex;
			//! ���Ɋ��蓖�Ă�ID�B
			int m_nNextId = 0;
			//! �t�@�C������ID��R�Â���}�b�v�B
			std::unordered_map<std::string, int> m_idMap;
			//! ID�ƃ��f���f�[�^��R�Â���}�b�v�B
			std::map<int, std::shared_ptr<ModelData>> m_modelMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_MODEL_MODELMANAGER_H___