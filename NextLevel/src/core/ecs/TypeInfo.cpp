#include "TypeInfo.h"

namespace ecs
{
	std::mutex TypeManager::m_Mutex;
	std::size_t TypeManager::m_NextId = 0;
}