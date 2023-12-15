#ifndef ___SRC_COMMON_CORE_ECS_COMPONENT_CDEBUGCAMERA_H___
#define ___SRC_COMMON_CORE_ECS_COMPONENT_CDEBUGCAMERA_H___

#include <Windows.h>

#include "../IComponentData.h"
#include <DirectXMath.h>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			struct CDebugCamera : public IComponentData
			{
				DirectX::XMFLOAT3 m_Pos = { 10.0f, 0.0f, 0.0f };
				DirectX::XMFLOAT3 m_Look = { 0.0f, 0.0f, 0.0f };
				DirectX::XMFLOAT3 m_Up = { 0.0f, 1.0f, 0.0f };
				float m_fNearClip = 0.1f;
				float m_fFarClip = 1000.0f;
				float m_fFovy = 60.0f;
				float m_fAspect = 16.0f / 9.0f;
				DirectX::XMFLOAT4X4 m_ViewMat;
				DirectX::XMFLOAT4X4 m_ProjMat;
				POINT m_oldPos;
				int m_state;
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_COMPONENT_CDEBUGCAMERA_H___