#ifndef ___SRC_COMMON_CORE_ECS_WORLD_H___
#define ___SRC_COMMON_CORE_ECS_WORLD_H___

#include <vector>
#include <memory>
#include "Chunk.h"

namespace NextLevel
{
	namespace ecs
	{
		class SystemBase;

		class World
		{
			friend class EntityManager;
		public:
			World();
			//~World();

			/**
			* @brief 更新処理を行います。
			* @param _deltaTime 前フレームからの経過時間。
			*/
			void Update(float _deltaTime);

			/**
			* @brief 描画処理を行います。
			*/
			void Draw();

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

			void ChangeUpdateOrder(
				const std::size_t& _oldUpdateOrder,
				const int& _oldVecIndex,
				const std::size_t& _newUpdateOrder)
			{
				m_SystemList[_newUpdateOrder].push_back(
					m_SystemList[_oldUpdateOrder][_oldVecIndex]);
				auto it = m_SystemList[_oldUpdateOrder].begin();
				for (int i = 0; i < _oldVecIndex; i++)
					++it;
				m_SystemList[_oldUpdateOrder].erase(it);
			}

			std::shared_ptr<EntityManager> GetEntityManager()
			{
				return m_pEntityManager;
			}

			void SetName(std::string _name)
			{
				m_szName = _name;
			}

			std::string GetName() const noexcept
			{
				return m_szName;
			}

		private:
			std::vector<Chunk> m_ChunkList;
			std::vector<std::vector<std::shared_ptr<SystemBase>>> m_SystemList;
			std::shared_ptr<EntityManager> m_pEntityManager;
			std::string m_szName;
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_WORLD_H___