#ifndef ___SRC_CORE_ECS_ENTITY_H___
#define ___SRC_CORE_ECS_ENTITY_H___

#include "Utils.h"

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
			: m_Idntifier(NULL)
		{
			utils::SetIndex(m_Idntifier, _index);
			utils::SetVersion(m_Idntifier, _version);
		}

		//! �G���e�B�e�B�̈�ӂ̎��ʃr�b�g�B
		utils::EntityIdentifier m_Idntifier;
	};
}

#endif //!___SRC_CORE_ECS_ENTITY_H___