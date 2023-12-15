#include "SBindDebugCamera.h"

#include "src/engine/graphics/windows/Graphics.h"

#include "../component/CDebugCamera.h"
#include "../component/CTransform.h"

using namespace NextLevel::ecs::component;

namespace NextLevel
{
	namespace ecs
	{
		namespace system
		{
			/**
			* @brief 初期化処理をします。
			*/
			void SBindDebugCamera::Init()
			{
				// アーキタイプに必要なコンポーネントを設定する。
				ArchetypeSetting();
			}

			/**
			* @brief 描画処理をします。
			*/
			void SBindDebugCamera::Draw()
			{
				// 該当のエンティティが持つ該当のコンポーネントのみに
				// 受け渡す関数を適用する。
				this->ExecuteForEntitiesMatching
					<CDebugCamera>
					([this](CDebugCamera& _camera)
						{
							//=== 行列の算出
							// ビュー行列の算出
							_camera.m_ViewMat =
								CalcViewMatrix(_camera);
							// プロジェクション行列の算出
							_camera.m_ProjMat =
								CalcProjectionMatrix(_camera);

							//=== 定数バッファへの書き込み
							// ビュー行列の書き込み
							CBufWriteViewMatrix(
								&_camera.m_ViewMat);
							CBufWriteProjectionMatrix(
								&_camera.m_ProjMat);
						});
			}

			/**
			* @brief アーキタイプに必要なコンポーネントを設定します。
			*/
			void SBindDebugCamera::ArchetypeSetting()
			{
				// カメラ情報など、描画には必要不可欠。
				m_Archetype.AddType<CDebugCamera>();
				// カメラの座標や向きは行列を作成する上で必要不可欠。
				m_Archetype.AddType<CTransform>();
			}

			/**
			* @brief Cameraからビュー行列を算出します。
			* @param _camera カメラデータ。
			* @return DirectX::XMFLOAT4X4 ビュー行列。
			*/
			DirectX::XMFLOAT4X4 SBindDebugCamera::CalcViewMatrix(component::CDebugCamera _camera)
			{
				DirectX::XMFLOAT4X4 viewMat;

				DirectX::XMStoreFloat4x4(&viewMat,
					DirectX::XMMatrixLookAtLH(
						DirectX::XMLoadFloat3(&_camera.m_Pos),
						DirectX::XMLoadFloat3(&_camera.m_Look),
						DirectX::XMLoadFloat3(&_camera.m_Up)
					));

				return viewMat;
			}

			/**
			* @brief Cameraからプロジェクション行列を算出します。
			* @param _camera カメラデータ。
			* @return DirectX::XMFLOAT4X4 プロジェクション行列。
			*/
			DirectX::XMFLOAT4X4 SBindDebugCamera::CalcProjectionMatrix(component::CDebugCamera _camera)
			{
				DirectX::XMFLOAT4X4 projMat;

				// プロジェクションマトリクスの算出
				DirectX::XMStoreFloat4x4(&projMat,
					DirectX::XMMatrixPerspectiveFovLH(
						_camera.m_fFovy / 180.0f * 3.14f,
						_camera.m_fAspect,
						_camera.m_fNearClip,
						_camera.m_fFarClip
					));

				return projMat;
			}

			/**
			* @brief ビュー行列を定数バッファに書き込みます。
			* @param _viewMat ビュー行列。
			*/
			void SBindDebugCamera::CBufWriteViewMatrix(DirectX::XMFLOAT4X4* _viewMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()
					->SetViewMatrix(_viewMat);
			}

			/**
			* @brief プロジェクション行列を定数バッファに書き込みます。
			* @param _projMat プロジェクション行列。
			*/
			void SBindDebugCamera::CBufWriteProjectionMatrix(DirectX::XMFLOAT4X4* _projMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()
					->SetProjectionMatrix(_projMat);
			}
		}
	}
}