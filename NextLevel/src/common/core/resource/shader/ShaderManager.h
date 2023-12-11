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
		* @brief �V�F�[�_�[���Ǘ�����N���X�B
		*/
		class ShaderManager {
		private:
			/**
			* @brief �R���X�g���N�^�B
			*/
			ShaderManager() = default;

			/**
			* @brief �f�X�g���N�^�B
			*/
			~ShaderManager() = default;

		public:
			/**
			 * @brief �V���O���g���C���X�^���X���擾���܂��B
			 * @return ShaderManager& �V�F�[�_�[�}�l�[�W���[�N���X�̃C���X�^���X�B
			 */
			static ShaderManager& GetInstance()
			{
				static ShaderManager instance;
				return instance;
			}

		public:
			/**
			* @brief �V�F�[�_�[ID���擾���܂��B�B���ǂݍ��݂ł���΃��[�h���܂��B
			* @param _kind �V�F�[�_�[�̎�ށB
			* @param _fileName �V�F�[�_�[�̃t�@�C�����B
			* @return int �V�F�[�_�[�Ɋ��蓖�Ă�ꂽID�B
			*/
			std::optional<int> GetAndLoadShaderID(ShaderData::EnKind _kind, std::string _fileName)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// ���ǂݍ��݂���T��
				auto it = m_idMap.find(_fileName);
				if (it != m_idMap.end()) return it->second;

				// �V���ɓǂݍ���
				std::shared_ptr<ShaderData> newShader =
					std::make_shared<ShaderData>();
				if (!newShader->Load(_kind, _fileName.c_str())) return {};

				// ���蓖��
				int newId = m_nNextId;
				m_idMap[_fileName] = newId;
				m_shaderMap[newId] = newShader;
				m_nNextId++;

				return newId;
			}

			/**
			* @brief �V�F�[�_�[�f�[�^���擾���܂��B
			* @param _id �V�F�[�_�[��ID�B
			* @return �V�F�[�_�[�f�[�^�B
			*/
			std::optional<std::shared_ptr<ShaderData>> GetShaderData(int _id)
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				// ���ǂݍ��݂���T��
				auto it = m_shaderMap.find(_id);
				if (it != m_shaderMap.end()) return it->second;

				return {};
			}

			/**
			* @brief �S�ẴV�F�[�_�[���擾���܂��B
			* @return std::map<int, std::shared_ptr<ShaderData>>& �V�F�[�_�[�}�b�v�B
			*/
			const std::map<int, std::shared_ptr<ShaderData>>& GetShaderDatas()
			{
				std::lock_guard<std::mutex> lock(m_mutex);

				return m_shaderMap;
			}

		private:
			//! �~���[�e�b�N�X�B
			std::mutex m_mutex;
			//! ���Ɋ��蓖�Ă�ID�B
			int m_nNextId = 0;
			//! �t�@�C������ID��R�Â���}�b�v�B
			std::unordered_map<std::string, int> m_idMap;
			//! ID�ƃV�F�[�_�[�f�[�^��R�Â���}�b�v�B
			std::map<int, std::shared_ptr<ShaderData>> m_shaderMap;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERMANAGER_H___