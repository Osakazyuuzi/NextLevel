#ifndef ___SRC_COMMON_CORE_ECS_SYSTEM_SOPERATIONDEBUGCAMERA_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEM_SOPERATIONDEBUGCAMERA_H___

#include "../SystemBase.h"

#include <DirectXMath.h>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			class CDebugCamera;
		}

		namespace system
		{
			class SOperationDebugCamera : public SystemBase {
			public:
				/**
				* @brief �R���X�g���N�^�B
				* @param _pWorld ���[���h�|�C���^�B
				*/
				SOperationDebugCamera(World* _pWorld)
					: SystemBase(_pWorld)
				{}

				/**
				* @brief ���������������܂��B
				*/
				void Init() override;

				/**
				* @brief �X�V���������܂��B
				*/
				void Update(float _deltaTime) override;

			private:
				// ������
				struct Argument
				{
					DirectX::XMFLOAT2 mouseMove;
					DirectX::XMVECTOR vCamFront;
					DirectX::XMVECTOR vCamSide;
					DirectX::XMVECTOR vCamUp;
					DirectX::XMVECTOR vCamPos;
					DirectX::XMVECTOR vCamLook;
					float focus;
				};
				// ������@
				enum CameraDCCKind {
					CAM_DCC_NONE,
					CAM_DCC_ORBIT,
					CAM_DCC_TRACK,
					CAM_DCC_DOLLY,
					CAM_DCC_FLIGHT,
				};
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___