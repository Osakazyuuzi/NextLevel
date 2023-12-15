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
			* @brief ���������������܂��B
			*/
			void SBindDebugCamera::Init()
			{
				// �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肷��B
				ArchetypeSetting();
			}

			/**
			* @brief �`�揈�������܂��B
			*/
			void SBindDebugCamera::Draw()
			{
				// �Y���̃G���e�B�e�B�����Y���̃R���|�[�l���g�݂̂�
				// �󂯓n���֐���K�p����B
				this->ExecuteForEntitiesMatching
					<CDebugCamera>
					([this](CDebugCamera& _camera)
						{
							//=== �s��̎Z�o
							// �r���[�s��̎Z�o
							_camera.m_ViewMat =
								CalcViewMatrix(_camera);
							// �v���W�F�N�V�����s��̎Z�o
							_camera.m_ProjMat =
								CalcProjectionMatrix(_camera);

							//=== �萔�o�b�t�@�ւ̏�������
							// �r���[�s��̏�������
							CBufWriteViewMatrix(
								&_camera.m_ViewMat);
							CBufWriteProjectionMatrix(
								&_camera.m_ProjMat);
						});
			}

			/**
			* @brief �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肵�܂��B
			*/
			void SBindDebugCamera::ArchetypeSetting()
			{
				// �J�������ȂǁA�`��ɂ͕K�v�s���B
				m_Archetype.AddType<CDebugCamera>();
				// �J�����̍��W������͍s����쐬�����ŕK�v�s���B
				m_Archetype.AddType<CTransform>();
			}

			/**
			* @brief Camera����r���[�s����Z�o���܂��B
			* @param _camera �J�����f�[�^�B
			* @return DirectX::XMFLOAT4X4 �r���[�s��B
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
			* @brief Camera����v���W�F�N�V�����s����Z�o���܂��B
			* @param _camera �J�����f�[�^�B
			* @return DirectX::XMFLOAT4X4 �v���W�F�N�V�����s��B
			*/
			DirectX::XMFLOAT4X4 SBindDebugCamera::CalcProjectionMatrix(component::CDebugCamera _camera)
			{
				DirectX::XMFLOAT4X4 projMat;

				// �v���W�F�N�V�����}�g���N�X�̎Z�o
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
			* @brief �r���[�s���萔�o�b�t�@�ɏ������݂܂��B
			* @param _viewMat �r���[�s��B
			*/
			void SBindDebugCamera::CBufWriteViewMatrix(DirectX::XMFLOAT4X4* _viewMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()
					->SetViewMatrix(_viewMat);
			}

			/**
			* @brief �v���W�F�N�V�����s���萔�o�b�t�@�ɏ������݂܂��B
			* @param _projMat �v���W�F�N�V�����s��B
			*/
			void SBindDebugCamera::CBufWriteProjectionMatrix(DirectX::XMFLOAT4X4* _projMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()
					->SetProjectionMatrix(_projMat);
			}
		}
	}
}