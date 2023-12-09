#ifndef ___SRC_COMMON_CORE_ECS_WORLD_H___
#define ___SRC_COMMON_CORE_ECS_WORLD_H___

#include <vector>
#include <memory>
#include "Chunk.h"
#include "SystemBase.h"

namespace NextLevel
{
	namespace ecs
	{
		class World
		{
			friend class EntityManager;
		public:
			World();
			//~World();

			void Update(float _deltaTime)
			{
				for (auto systems : m_SystemList)
				{
					for (auto&& system : systems)
					{
						system->Update(_deltaTime);
					}
				}
			}

			template <typename SystemT>
			void AddSystem(const std::size_t& _updateOrder)
			{
				std::size_t size = _updateOrder - m_SystemList.size();
				if (size > 0 || !m_SystemList.size())
				{
					m_SystemList.resize(_updateOrder + 1);
				}

				m_SystemList[_updateOrder].push_back(std::make_shared<SystemT>(this));
				m_SystemList[_updateOrder].back()->Init();
			}

			EntityManager* GetEntityManager()
			{
				return m_pEntityManager.get();
			}

		private:
			std::vector<Chunk> m_ChunkList;
			std::vector<std::vector<std::shared_ptr<SystemBase>>> m_SystemList;
			std::shared_ptr<EntityManager> m_pEntityManager;
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_WORLD_H___