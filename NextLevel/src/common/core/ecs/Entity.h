#ifndef ___SRC_COMMON_CORE_ECS_ENTITY_H___
#define ___SRC_COMMON_CORE_ECS_ENTITY_H___

#include "Utils.h"

namespace NextLevel
{
	namespace ecs
	{
		/**
		* @struct Entity
		* @brief ECS�A�[�L�e�N�`���ɂ�����G���e�B�e�B��\���\���́B
		*/
		struct Entity
		{
			/**
			* @brief �R���X�g���N�^
			* @param _index �G���e�B�e�B�̃C���f�b�N�X�B
			* @param _version �G���e�B�e�B�̃o�[�W����
			*/
			Entity(const std::uint32_t _index, const std::uint32_t _version)
				: m_Identifier(NULL)
			{
				utils::SetIndex(m_Identifier, _index);
				utils::SetVersion(m_Identifier, _version);
			}

			/**
			* @brief �R���X�g���N�^
			* @param _Identifier �G���e�B�e�B�̎��ʃr�b�g�B
			*/
			Entity(const utils::EntityIdentifier _Identifier)
				: m_Identifier(_Identifier)
			{}

			//! �G���e�B�e�B�̈�ӂ̎��ʃr�b�g�B
			utils::EntityIdentifier m_Identifier;
		};
	}
}
#endif //!___SRC_COMMON_CORE_ECS_ENTITY_H___