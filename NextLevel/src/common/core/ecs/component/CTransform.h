#ifndef ___SRC_COMMON_CORE_ECS_COMPONENT_CTRANSFORM_H___
#define ___SRC_COMMON_CORE_ECS_COMPONENT_CTRANSFORM_H___

#include "../IComponentData.h"
#include <Eigen/Dense>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			struct CTransform : public IComponentData
			{
				Eigen::Vector3f m_position = { 0.0f, 0.0f, 0.0f };
				Eigen::Vector3f m_rotation = { 0.0f, 0.0f, 0.0f };
				Eigen::Vector3f m_scale = { 1.0f, 1.0f, 1.0f };
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_COMPONENT_CTRANSFORM_H___