#ifndef ___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___

#include "Archetype.h"
#include "Chunk.h"
#include "World.h"
#include "EntityManager.h"

namespace NextLevel
{
	namespace ecs
	{
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
			virtual void Init() {}

			/**
			* @brief ���������s���܂��B
			* @param _deltaTime �O�t���[���Ƃ̍�[ms]
			*/
			virtual void Update(float _deltaTime) {}

			/**
			* @brief �`������s���܂��B
			*/
			virtual void Draw() {}

			/**
			* @brief ���s����̂ɕK�v�Ƃ���A�[�L�^�C�v���擾���܂��B
			* @return const Archetype& ���s����̂ɕK�v�ȃA�[�L�^�C�v�B
			*/
			inline const Archetype& GetArchetype() const noexcept
			{
				return m_Archetype;
			}

			/**
			* @brief ���s����̂ɕK�v�Ƃ���A�[�L�^�C�v���擾���܂��B
			* @return Archetype* ���s����̂ɕK�v�ȃA�[�L�^�C�v�B
			*/
			inline Archetype* GetArchetype()
			{
				return &m_Archetype;
			}

			/**
			* @brief �����Ă��郏�[���h�̃G���e�B�e�B�}�l�[�W���[���擾���܂��B
			* @return std::shared_ptr<EntityManager> �����Ă��郏�[���h�̃G���e�B�e�B�}�l�[�W���[�B
			*/
			std::shared_ptr<EntityManager> GetEntityManager();

		protected:
			/**
			* @brief �K�v�ȃA�[�L�^�C�v���܂�ł���G���e�B�e�B�Ɋ֐������s���܂��B
			* @tparam T1 �R���|�[�l���g�̌^�B
			* @param _func ���s����֐��B
			*/
			template <class T1, typename Func>
			void ExecuteForEntitiesMatching(Func&& _func)
			{
				// �A�[�L�^�C�v���܂܂�Ă���`�����N���X�g���擾
				auto pChunkList = m_pWorld->GetEntityManager()->GetContainChunkList(m_Archetype);

				for (auto&& pChunk : pChunkList)
				{
					// �K�v�ȃR���|�[�l���g�Q�𔲂��o��
					auto arg1 = pChunk->GetComponentList<T1>();

					// ���������s
					ExecuteForEntitiesMatchingImpl(pChunk, _func, arg1);
				}
			}

			/**
			* @brief �K�v�ȃA�[�L�^�C�v���܂�ł���G���e�B�e�B�Ɋ֐������s���܂��B
			* @tparam T1 �R���|�[�l���g�̌^�B
			* @tparam T2 �R���|�[�l���g�̌^�B
			* @param _func ���s����֐��B
			*/
			template <class T1, class T2, typename Func>
			void ExecuteForEntitiesMatching(Func&& _func)
			{
				// �A�[�L�^�C�v���܂܂�Ă���`�����N���X�g���擾
				auto pChunkList = m_pWorld->GetEntityManager()->GetContainChunkList(m_Archetype);

				for (auto&& pChunk : pChunkList)
				{
					// �K�v�ȃR���|�[�l���g�Q�𔲂��o��
					auto arg1 = pChunk->GetComponentList<T1>();
					auto arg2 = pChunk->GetComponentList<T2>();

					// ���������s
					ExecuteForEntitiesMatchingImpl(pChunk, _func, arg1, arg2);
				}
			}

		private:
			/**
			* @brief �S�Ă̊Y���G���e�B�e�B�Ɋ֐������s���܂��B
			* @param _pChunk �Y���`�����N�B
			* @param _func ���s����֐��B
			* @param _args �֐��Ɉ����n���R���|�[�l���g�Q�B
			*/
			template <typename Func, class... Args>
			static void ExecuteForEntitiesMatchingImpl(
				Chunk* _pChunk,
				Func&& _func,
				Args... _args)
			{
				for (std::uint32_t i = 0; i < _pChunk->GetSize(); ++i)
				{
					_func(_args[i]...);
				}
			}

		protected:
			//! ���s����̂ɕK�v�Ƃ���A�[�L�^�C�v�B
			Archetype m_Archetype;
		private:
			//! �����Ă��郏�[���h�ւ̃|�C���^�B
			World* m_pWorld = nullptr;
		};
	}
}
#endif //!___SRC_COMMON_CORE_ECS_SYSTEMBASE_H___