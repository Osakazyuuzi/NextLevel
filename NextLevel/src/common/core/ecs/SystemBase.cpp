#include "SystemBase.h"

#include "World.h"
#include "EntityManager.h"
#include "Chunk.h"

namespace NextLevel
{
	namespace ecs
	{
		std::shared_ptr<EntityManager> SystemBase::GetEntityManager()
		{
			return m_pWorld->GetEntityManager();
		}
	}
}