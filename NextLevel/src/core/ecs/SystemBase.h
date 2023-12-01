#ifndef ___SRC_CORE_ECS_SYSTEMBASE_H___
#define ___SRC_CORE_ECS_SYSTEMBASE_H___

#include "Archetype.h"

namespace ecs
{
	class World;
	class EntityManager;

	/**
	* @class SystemBase
	* @brief �V�X�e���x�[�X�N���X�B�S�ẴV�X�e���͂�����p������B
	*/
	class SystemBase
	{
	public:
		/**
		* @brief �R���X�g���N�^�B
		* @param _pWorld �����郏�[���h�ւ̃|�C���^�B
		*/
		SystemBase(World* _pWorld)
			: m_pWorld(_pWorld)
		{}

		/**
		* @brief �ŏ��Ɉ�x�̂ݏ��������s���܂��B
		*/
		virtual void Init() = 0;

		/**
		* @brief ���t���[�����������s���܂��B
		* @param _deltaTime �O�t���[���Ƃ̍�[ms]
		*/
		virtual void Update(float _deltaTime) = 0;

		/**
		* @brief ���s����̂ɕK�v�Ƃ���A�[�L�^�C�v���擾���܂��B
		* @return ���s����̂ɕK�v�ȃA�[�L�^�C�v�B
		*/
		const Archetype& GetArchetype() const noexcept
		{
			return m_Archetype;
		}

		/**
		* @brief �����Ă��郏�[���h�̃G���e�B�e�B�}�l�[�W���[���擾���܂��B
		* @return EntityManager* �����Ă��郏�[���h�̃G���e�B�e�B�}�l�[�W���[�B
		*/
		EntityManager* GetEntityManager();

	protected:
		//! ���s����̂ɕK�v�Ƃ���A�[�L�^�C�v�B
		Archetype m_Archetype;

	private:
		//! �����Ă��郏�[���h�ւ̃|�C���^�B
		World* m_pWorld = nullptr;
	};
}

#endif //!___SRC_CORE_ECS_SYSTEMBASE_H___