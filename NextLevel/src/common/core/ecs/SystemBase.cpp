#include "SystemBase.h"
#include "World.h"

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