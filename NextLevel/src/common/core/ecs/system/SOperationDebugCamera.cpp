#include "SOperationDebugCamera.h"

#include "../component/CDebugCamera.h"

#include "src/engine/input/windows/Input.h"

namespace NextLevel
{
	namespace ecs
	{
		namespace system
		{
			void SOperationDebugCamera::Init()
			{
				m_Archetype.AddType<component::CDebugCamera>();
			}

			void SOperationDebugCamera::Update(float _deltaTime)
			{
				// 該当のエンティティが持つ該当のコンポーネントのみに
				// 受け渡す関数を適用する。
				this->ExecuteForEntitiesMatching
					<component::CDebugCamera>
					([this](component::CDebugCamera& _camera)
						{
							CameraDCCKind prev = (CameraDCCKind)_camera.m_state;
							if (IsKeyPress(VK_MENU))
							{
								_camera.m_state = CAM_DCC_NONE;
								if (IsKeyPress(VK_LBUTTON)) _camera.m_state = CAM_DCC_ORBIT;
								if (IsKeyPress(VK_MBUTTON)) _camera.m_state = CAM_DCC_TRACK;
								if (IsKeyPress(VK_RBUTTON)) _camera.m_state = CAM_DCC_DOLLY;
							}
							else if (IsKeyPress(VK_RBUTTON))
							{
								_camera.m_state = CAM_DCC_FLIGHT;
							}
							else
							{
								_camera.m_state = CAM_DCC_NONE;
							}
							if (prev != _camera.m_state)
							{
								GetCursorPos(&_camera.m_oldPos);
							}
							if (_camera.m_state == CAM_DCC_NONE) return;

							Argument arg;
							// マウス移動量
							POINT cursorPos;
							GetCursorPos(&cursorPos);
							arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - _camera.m_oldPos.x, (float)cursorPos.y - _camera.m_oldPos.y);
							_camera.m_oldPos = cursorPos;
							// カメラ情報
							arg.vCamPos = DirectX::XMLoadFloat3(&_camera.m_Pos);
							arg.vCamLook = DirectX::XMLoadFloat3(&_camera.m_Look);
							DirectX::XMVECTOR vCamUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&_camera.m_Up));
							DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
							// カメラ姿勢
							arg.vCamFront = DirectX::XMVector3Normalize(vFront);
							arg.vCamSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
							arg.vCamUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
							// フォーカス距離
							DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));

							switch (_camera.m_state)
							{
							case CAM_DCC_ORBIT:
							{
								// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
								float angleX = 360.0f * arg.mouseMove.x / 1220.0f;
								float angleY = 180.0f * arg.mouseMove.y / 660.0f;

								// Y軸回転でカメラの横軸を回転
								DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
								DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

								// 回転後の軸で縦回転
								DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
								DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
								vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

								// カメラ座標更新
								DirectX::XMStoreFloat3(&_camera.m_Pos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

								// アップベクトル更新
								DirectX::XMStoreFloat3(&_camera.m_Up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));

								break;
							}
							case CAM_DCC_TRACK:
							{
								// 高さA、底辺Bとする三角形について tanΘ = A / Bが成り立つ
								// 上記式をもとに割り出した遠景の高さを、移動量 / 画面サイズ の比率から、移動量として求める
								float farScreenHeight = tanf(_camera.m_fFovy * 0.5f) * _camera.m_fFarClip;
								float screenRateW = arg.mouseMove.x / 640.0f;
								float screenRateH = arg.mouseMove.y / 360.0f;
								float farMoveX = -farScreenHeight * screenRateW * _camera.m_fAspect;
								float farMoveY = farScreenHeight * screenRateH;

								// カメラの姿勢をもとに移動
								float rate = arg.focus / _camera.m_fFarClip;
								DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
								vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamSide, farMoveX * rate));
								vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorScale(arg.vCamUp, farMoveY * rate));
								DirectX::XMStoreFloat3(&_camera.m_Pos, DirectX::XMVectorAdd(arg.vCamPos, vCamMove));
								DirectX::XMStoreFloat3(&_camera.m_Look, DirectX::XMVectorAdd(arg.vCamLook, vCamMove));
								break;
							}
							case CAM_DCC_DOLLY:
							{
								float rate = arg.focus / _camera.m_fFarClip;
								DirectX::XMVECTOR vMove = DirectX::XMVectorScale(arg.vCamFront, _camera.m_fFarClip * 0.01f * rate * (arg.mouseMove.x + arg.mouseMove.y));
								DirectX::XMStoreFloat3(&_camera.m_Pos, DirectX::XMVectorAdd(arg.vCamPos, vMove));
								break;
							}
							case CAM_DCC_FLIGHT:
							{
								// マウスの移動量 / 画面サイズ の比率から、画面全体でどれだけ回転するか指定する。
								float angleX = 360.0f * arg.mouseMove.x / 1220.0f;
								float angleY = 180.0f * arg.mouseMove.y / 660.0f;

								// 横回転
								DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
								DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

								// 縦回転
								DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
								DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

								// キー入力で移動
								DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
								if (IsKeyPress('W')) vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
								if (IsKeyPress('S')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
								if (IsKeyPress('A')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
								if (IsKeyPress('D')) vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
								if (IsKeyPress('Q')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
								if (IsKeyPress('E')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
								vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f);

								// 更新
								DirectX::XMVECTOR vCamPos = DirectX::XMVectorAdd(arg.vCamPos, vCamMove);
								DirectX::XMStoreFloat3(&_camera.m_Pos, vCamPos);
								DirectX::XMStoreFloat3(&_camera.m_Look, DirectX::XMVectorAdd(vCamPos, DirectX::XMVectorScale(vFrontAxis, arg.focus)));
								DirectX::XMStoreFloat3(&_camera.m_Up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vFrontAxis, vSideAxis)));
								break;
							}
							}
						});
			}
		}
	}
}