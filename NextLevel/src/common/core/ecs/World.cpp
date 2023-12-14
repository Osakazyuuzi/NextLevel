#include "World.h"

#include "EntityManager.h"
#include "SystemBase.h"

namespace NextLevel
{
	namespace ecs
	{
		World::World()
		{
			m_pEntityManager = std::make_shared<EntityManager>(this);
		}

		/**
		* @brief �X�V�������s���܂��B
		* @param _deltaTime �O�t���[������̌o�ߎ��ԁB
		*/
		void World::Update(float _deltaTime)
		{
			for (auto systems : m_SystemList)
			{
				for (auto&& system : systems)
				{
					system->Update(_deltaTime);
				}
			}
		}

		/**
		* @brief �`�揈�����s���܂��B
		*/
		void World::Draw()
		{
			for (auto systems : m_SystemList)
			{
				for (auto&& system : systems)
				{
					system->Draw();
				}
			}
		}
	}
}