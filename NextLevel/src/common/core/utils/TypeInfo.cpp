#include "TypeInfo.h"

namespace NextLevel
{
	std::mutex TypeManager::m_Mutex;
	std::size_t TypeManager::m_NextId = 0;
}