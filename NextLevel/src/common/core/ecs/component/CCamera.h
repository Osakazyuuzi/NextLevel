#ifndef ___SRC_COMMON_CORE_ECS_COMPONENT_CCAMERA_H___
#define ___SRC_COMMON_CORE_ECS_COMPONENT_CCAMERA_H___

#include "../IComponentData.h"
#include <DirectXMath.h>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			struct CCamera : public IComponentData
			{
				float m_fNearClip = 0.1f;
				float m_fFarClip = 1000.0f;
				float m_fFovy = 60.0f;
				float m_fAspect = 16.0f / 9.0f;
				DirectX::XMFLOAT4X4 m_ViewMat;
				DirectX::XMFLOAT4X4 m_ProjMat;
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_COMPONENT_CCAMERA_H___