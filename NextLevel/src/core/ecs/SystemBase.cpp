#include "SystemBase.h"
#include "World.h"

namespace ecs
{
	EntityManager* SystemBase::GetEntityManager()
	{
		return m_pWorld->GetEntityManager();
	}
}