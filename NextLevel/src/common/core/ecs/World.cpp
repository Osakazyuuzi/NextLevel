#include "World.h"

#include "EntityManager.h"

namespace NextLevel
{
	namespace ecs
	{
		World::World()
		{
			m_pEntityManager = std::make_shared<EntityManager>(this);
		}
	}
}