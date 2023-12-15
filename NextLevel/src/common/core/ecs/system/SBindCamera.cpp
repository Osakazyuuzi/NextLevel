#include "SBindCamera.h"

#include "src/engine/graphics/windows/Graphics.h"

#include "../component/CCamera.h"
#include "../component/CTransform.h"

#include "src/common/core/utils/DXOperator.h"

using namespace NextLevel::ecs::component;

namespace NextLevel
{
	namespace ecs
	{
		/**
		* @brief 初期化処理をします。
		*/
		void SBindCamera::Init()
		{
			// アーキタイプに必要なコンポーネントを設定する。
			ArchetypeSetting();
		}

		/**
		* @brief 描画処理をします。
		*/
		void SBindCamera::Draw()
		{
			// 該当のエンティティが持つ該当のコンポーネントのみに
			// 受け渡す関数を適用する。
			this->ExecuteForEntitiesMatching
				<CCamera, CTransform>
				([this](CCamera& _camera, CTransform& _transform)
					{
						//=== 行列の算出
						// ビュー行列の算出
						_camera.m_ViewMat =
							Convert(CalcViewMatrix(_transform));
						// プロジェクション行列の算出
						_camera.m_ProjMat =
							Convert(CalcProjectionMatrix(_camera));

						//=== 定数バッファへの書き込み
						// ビュー行列の書き込み
						CBufWriteViewMatrix(
							Convert(_camera.m_ViewMat));
						CBufWriteProjectionMatrix(
							Convert(_camera.m_ProjMat));
					});
		}

		/**
		* @brief アーキタイプに必要なコンポーネントを設定します。
		*/
		void SBindCamera::ArchetypeSetting()
		{
			// カメラ情報など、描画には必要不可欠。
			m_Archetype.AddType<CCamera>();
			// カメラの座標や向きは行列を作成する上で必要不可欠。
			m_Archetype.AddType<CTransform>();
		}

		/**
		* @brief Cameraからビュー行列を算出します。
		* @param _transform 形状データ。
		* @return DirectX::XMMATRIX ビュー行列。
		*/
		DirectX::XMMATRIX SBindCamera::CalcViewMatrix(component::CTransform _transform)
		{
			DirectX::XMFLOAT4X4 viewMat;

			//=== 座標
			DirectX::XMFLOAT3 pos =
			{
				_transform.m_position.x(),
				_transform.m_position.y(),
				_transform.m_position.z()
			};

			//=== 注視点（正面方向）
			DirectX::XMFLOAT3 forward;
			DirectX::XMMATRIX rotMat;
			// 回転行列を求める
			rotMat = DirectX::XMMatrixRotationRollPitchYaw(
				_transform.m_rotation.x(),
				_transform.m_rotation.y(),
				_transform.m_rotation.z()
			);
			// 方面方向のベクトルを抽出
			DirectX::XMStoreFloat3(&forward, rotMat.r[2]);

			DirectX::XMFLOAT3 look =
			{
				_transform.m_position.x() + forward.x,
				_transform.m_position.y() + forward.y,
				_transform.m_position.z() + forward.z
			};

			//=== アップベクトル
			DirectX::XMFLOAT3 up;
			DirectX::XMStoreFloat3(&up, rotMat.r[1]);

			// ビュー行列の算出
			DirectX::XMStoreFloat4x4(
				&viewMat,
				DirectX::XMMatrixLookAtLH(
					DirectX::XMLoadFloat3(&pos),
					DirectX::XMLoadFloat3(&look),
					DirectX::XMLoadFloat3(&up)
				));

			return DirectX::XMLoadFloat4x4(&viewMat);
		}

		/**
		* @brief Cameraからプロジェクション行列を算出します。
		* @param _camera カメラデータ。
		* @return DirectX::XMMATRIX プロジェクション行列。
		*/
		DirectX::XMMATRIX SBindCamera::CalcProjectionMatrix(component::CCamera _camera)
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

			return DirectX::XMLoadFloat4x4(&projMat);
		}

		/**
		* @brief ビュー行列を定数バッファに書き込みます。
		* @param _viewMat ビュー行列。
		*/
		void SBindCamera::CBufWriteViewMatrix(DirectX::XMMATRIX _viewMat)
		{
			DirectX::XMFLOAT4X4 viewMat;

			DirectX::XMStoreFloat4x4(&viewMat, _viewMat);

			graphics::Graphics::GetInstance().GetRenderContext()
				->SetViewMatrix(&viewMat);
		}

		/**
		* @brief プロジェクション行列を定数バッファに書き込みます。
		* @param _projMat プロジェクション行列。
		*/
		void SBindCamera::CBufWriteProjectionMatrix(DirectX::XMMATRIX _projMat)
		{
			DirectX::XMFLOAT4X4 projMat;

			DirectX::XMStoreFloat4x4(&projMat, _projMat);

			graphics::Graphics::GetInstance().GetRenderContext()
				->SetProjectionMatrix(&projMat);
		}
	}
}