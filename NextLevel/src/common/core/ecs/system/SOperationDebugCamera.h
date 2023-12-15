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
				* @brief コンストラクタ。
				* @param _pWorld ワールドポインタ。
				*/
				SOperationDebugCamera(World* _pWorld)
					: SystemBase(_pWorld)
				{}

				/**
				* @brief 初期化処理をします。
				*/
				void Init() override;

				/**
				* @brief 更新処理をします。
				*/
				void Update(float _deltaTime) override;

			private:
				// 操作情報
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
				// 操作方法
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