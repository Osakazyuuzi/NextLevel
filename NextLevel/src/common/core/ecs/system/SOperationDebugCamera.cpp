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
				// �Y���̃G���e�B�e�B�����Y���̃R���|�[�l���g�݂̂�
				// �󂯓n���֐���K�p����B
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
							// �}�E�X�ړ���
							POINT cursorPos;
							GetCursorPos(&cursorPos);
							arg.mouseMove = DirectX::XMFLOAT2((float)cursorPos.x - _camera.m_oldPos.x, (float)cursorPos.y - _camera.m_oldPos.y);
							_camera.m_oldPos = cursorPos;
							// �J�������
							arg.vCamPos = DirectX::XMLoadFloat3(&_camera.m_Pos);
							arg.vCamLook = DirectX::XMLoadFloat3(&_camera.m_Look);
							DirectX::XMVECTOR vCamUp = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&_camera.m_Up));
							DirectX::XMVECTOR vFront = DirectX::XMVectorSubtract(arg.vCamLook, arg.vCamPos);
							// �J�����p��
							arg.vCamFront = DirectX::XMVector3Normalize(vFront);
							arg.vCamSide = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(vCamUp, arg.vCamFront));
							arg.vCamUp = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(arg.vCamFront, arg.vCamSide));
							// �t�H�[�J�X����
							DirectX::XMStoreFloat(&arg.focus, DirectX::XMVector3Length(vFront));

							switch (_camera.m_state)
							{
							case CAM_DCC_ORBIT:
							{
								// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
								float angleX = 360.0f * arg.mouseMove.x / 1220.0f;
								float angleY = 180.0f * arg.mouseMove.y / 660.0f;

								// Y����]�ŃJ�����̉�������]
								DirectX::XMMATRIX matRotY = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
								DirectX::XMVECTOR vAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matRotY));

								// ��]��̎��ŏc��]
								DirectX::XMMATRIX matRotSide = DirectX::XMMatrixRotationAxis(vAxis, DirectX::XMConvertToRadians(angleY));
								DirectX::XMVECTOR vRelative = DirectX::XMVectorScale(arg.vCamFront, arg.focus);
								vRelative = DirectX::XMVector3TransformCoord(vRelative, matRotY * matRotSide);

								// �J�������W�X�V
								DirectX::XMStoreFloat3(&_camera.m_Pos, DirectX::XMVectorSubtract(arg.vCamLook, vRelative));

								// �A�b�v�x�N�g���X�V
								DirectX::XMStoreFloat3(&_camera.m_Up, DirectX::XMVector3Normalize(DirectX::XMVector3Cross(DirectX::XMVector3Normalize(vRelative), vAxis)));

								break;
							}
							case CAM_DCC_TRACK:
							{
								// ����A�A���B�Ƃ���O�p�`�ɂ��� tan�� = A / B�����藧��
								// ��L�������ƂɊ���o�������i�̍������A�ړ��� / ��ʃT�C�Y �̔䗦����A�ړ��ʂƂ��ċ��߂�
								float farScreenHeight = tanf(_camera.m_fFovy * 0.5f) * _camera.m_fFarClip;
								float screenRateW = arg.mouseMove.x / 640.0f;
								float screenRateH = arg.mouseMove.y / 360.0f;
								float farMoveX = -farScreenHeight * screenRateW * _camera.m_fAspect;
								float farMoveY = farScreenHeight * screenRateH;

								// �J�����̎p�������ƂɈړ�
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
								// �}�E�X�̈ړ��� / ��ʃT�C�Y �̔䗦����A��ʑS�̂łǂꂾ����]���邩�w�肷��B
								float angleX = 360.0f * arg.mouseMove.x / 1220.0f;
								float angleY = 180.0f * arg.mouseMove.y / 660.0f;

								// ����]
								DirectX::XMMATRIX matUpRot = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(angleX));
								DirectX::XMVECTOR vSideAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamSide, matUpRot));

								// �c��]
								DirectX::XMMATRIX matSideRot = DirectX::XMMatrixRotationAxis(vSideAxis, DirectX::XMConvertToRadians(angleY));
								DirectX::XMVECTOR vFrontAxis = DirectX::XMVector3Normalize(DirectX::XMVector3TransformCoord(arg.vCamFront, matUpRot * matSideRot));

								// �L�[���͂ňړ�
								DirectX::XMVECTOR vCamMove = DirectX::XMVectorZero();
								if (IsKeyPress('W')) vCamMove = DirectX::XMVectorAdd(vCamMove, vFrontAxis);
								if (IsKeyPress('S')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vFrontAxis);
								if (IsKeyPress('A')) vCamMove = DirectX::XMVectorSubtract(vCamMove, vSideAxis);
								if (IsKeyPress('D')) vCamMove = DirectX::XMVectorAdd(vCamMove, vSideAxis);
								if (IsKeyPress('Q')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
								if (IsKeyPress('E')) vCamMove = DirectX::XMVectorAdd(vCamMove, DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
								vCamMove = DirectX::XMVectorScale(vCamMove, 0.1f);

								// �X�V
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