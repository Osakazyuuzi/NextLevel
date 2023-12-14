#ifndef ___SRC_COMMON_CORE_ECS_COMPONENT_CMESHRENDERER_H___
#define ___SRC_COMMON_CORE_ECS_COMPONENT_CMESHRENDERER_H___

#include "../IComponentData.h"
#include <Eigen/Dense>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			struct CMeshRenderer : public IComponentData
			{
				int m_modelId = 1;
				int m_materialId = 0;
				int m_vertexShaderId = 1;
				int m_pixelShaderId = 2;
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_COMPONENT_CMESHRENDERER_H___