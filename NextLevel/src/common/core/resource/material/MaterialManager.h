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
		* @brief �}�e���A�����Ǘ�����N���X�B
		*/
		class MaterialManager {
		public:
			/**
			* @brief �}�e���A����V�K�쐬���܂��B
			* @return int �}�e���A���ɕR�Â�ID�B
			*/
			int CreateMaterial()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// �V���ɍ쐬
				int newId = m_nNextId++;
				m_materialMap[newId] = std::make_shared<MaterialData>();

				return newId;
			}

			/**
			* @brief �}�e���A��ID���擾���܂��B
			* @param _id �}�e���A��ID�B
			* @return std::optional<std::shared_ptr<MaterialData>> �}�e���A���f�[�^�B
			*/
			std::optional<std::shared_ptr<MaterialData>> GetMaterialData(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// �������X�g����T��
				auto it = m_materialMap.find(_id);
				if (it != m_materialMap.end()) return it->second;

				return {};
			}

			/**
			* @brief �}�e���A���}�b�v����}�e���A�����폜���܂��B
			* @param _id �}�e���A��ID�B
			*/
			void DestroyMaterial(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// �������X�g����T��
				auto it = m_materialMap.find(_id);
				if (it != m_materialMap.end()) m_materialMap.erase(it);
			}

			/**
			* @brief �S�Ẵ}�e���A�����擾���܂��B
			* @return std::map<int, std::shared_ptr<MaterialData>>& �}�e���A���}�b�v�B
			*/
			std::map<int, std::shared_ptr<MaterialData>>& GetMaterialDatas()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				return m_materialMap;
			}

		private:
			//! �~���[�e�b�N�X
			std::mutex m_mutex;
			//! ���Ɋ��蓖�Ă�ID�B
			int m_nNextId = 0;
			//! ID�ƃ}�e���A����R�Â���}�b�v
			std::map<int, std::shared_ptr<MaterialData>> m_materialMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALMANAGER_H___