#include "World.h"

#include "EntityManager.h"

namespace ecs
{
	World::World()
	{
		m_pEntityManager = std::make_shared<EntityManager>(this);
	}
}