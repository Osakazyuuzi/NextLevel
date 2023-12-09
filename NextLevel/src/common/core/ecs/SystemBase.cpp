#include "SystemBase.h"
#include "World.h"

namespace NextLevel
{
	namespace ecs
	{
		EntityManager* SystemBase::GetEntityManager()
		{
			return m_pWorld->GetEntityManager();
		}
	}
}