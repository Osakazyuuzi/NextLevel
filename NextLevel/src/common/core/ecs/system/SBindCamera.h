#ifndef ___SRC_COMMON_CORE_ECS_SYSTEM_SBINDCAMERA_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEM_SBINDCAMERA_H___

#include "../SystemBase.h"

#include <DirectXMath.h>

namespace NextLevel
{
	namespace ecs
	{
		namespace component
		{
			class CCamera;
			class CTransform;
		}

		class SBindCamera : public SystemBase {
		public:
			/**
			* @brief コンストラクタ。
			* @param _pWorld ワールドポインタ。
			*/
			SBindCamera(World* _pWorld)
				: SystemBase(_pWorld)
			{}

			/**
			* @brief 初期化処理をします。
			*/
			void Init() override;

			/**
			* @brief 描画処理をします。
			*/
			void Draw() override;

		private:
			/**
			* @brief アーキタイプに必要なコンポーネントを設定します。
			*/
			void ArchetypeSetting();

			/**
			* @brief Cameraからビュー行列を算出します。
			* @param _transform 形状データ。
			* @return DirectX::XMMATRIX ビュー行列。
			*/
			DirectX::XMMATRIX CalcViewMatrix(component::CTransform _transform);

			/**
			* @brief Cameraからプロジェクション行列を算出します。
			* @param _camera カメラデータ。
			* @return DirectX::XMMATRIX プロジェクション行列。
			*/
			DirectX::XMMATRIX CalcProjectionMatrix(component::CCamera _camera);
		
			/**
			* @brief ビュー行列を定数バッファに書き込みます。
			* @param _viewMat ビュー行列。
			*/
			void CBufWriteProjectionMatrix(DirectX::XMMATRIX _projMat);

			/**
			* @brief プロジェクション行列を定数バッファに書き込みます。
			* @param _projMat プロジェクション行列。
			*/
			void CBufWriteViewMatrix(DirectX::XMMATRIX _viewMat);
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___