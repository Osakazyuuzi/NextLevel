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
		* @brief ���������������܂��B
		*/
		void SBindCamera::Init()
		{
			// �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肷��B
			ArchetypeSetting();
		}

		/**
		* @brief �`�揈�������܂��B
		*/
		void SBindCamera::Draw()
		{
			// �Y���̃G���e�B�e�B�����Y���̃R���|�[�l���g�݂̂�
			// �󂯓n���֐���K�p����B
			this->ExecuteForEntitiesMatching
				<CCamera, CTransform>
				([this](CCamera& _camera, CTransform& _transform)
					{
						//=== �s��̎Z�o
						// �r���[�s��̎Z�o
						_camera.m_ViewMat =
							Convert(CalcViewMatrix(_transform));
						// �v���W�F�N�V�����s��̎Z�o
						_camera.m_ProjMat =
							Convert(CalcProjectionMatrix(_camera));

						//=== �萔�o�b�t�@�ւ̏�������
						// �r���[�s��̏�������
						CBufWriteViewMatrix(
							Convert(_camera.m_ViewMat));
						CBufWriteProjectionMatrix(
							Convert(_camera.m_ProjMat));
					});
		}

		/**
		* @brief �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肵�܂��B
		*/
		void SBindCamera::ArchetypeSetting()
		{
			// �J�������ȂǁA�`��ɂ͕K�v�s���B
			m_Archetype.AddType<CCamera>();
			// �J�����̍��W������͍s����쐬�����ŕK�v�s���B
			m_Archetype.AddType<CTransform>();
		}

		/**
		* @brief Camera����r���[�s����Z�o���܂��B
		* @param _transform �`��f�[�^�B
		* @return DirectX::XMMATRIX �r���[�s��B
		*/
		DirectX::XMMATRIX SBindCamera::CalcViewMatrix(component::CTransform _transform)
		{
			DirectX::XMFLOAT4X4 viewMat;

			//=== ���W
			DirectX::XMFLOAT3 pos =
			{
				_transform.m_position.x(),
				_transform.m_position.y(),
				_transform.m_position.z()
			};

			//=== �����_�i���ʕ����j
			DirectX::XMFLOAT3 forward;
			DirectX::XMMATRIX rotMat;
			// ��]�s������߂�
			rotMat = DirectX::XMMatrixRotationRollPitchYaw(
				_transform.m_rotation.x(),
				_transform.m_rotation.y(),
				_transform.m_rotation.z()
			);
			// ���ʕ����̃x�N�g���𒊏o
			DirectX::XMStoreFloat3(&forward, rotMat.r[2]);

			DirectX::XMFLOAT3 look =
			{
				_transform.m_position.x() + forward.x,
				_transform.m_position.y() + forward.y,
				_transform.m_position.z() + forward.z
			};

			//=== �A�b�v�x�N�g��
			DirectX::XMFLOAT3 up;
			DirectX::XMStoreFloat3(&up, rotMat.r[1]);

			// �r���[�s��̎Z�o
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
		* @brief Camera����v���W�F�N�V�����s����Z�o���܂��B
		* @param _camera �J�����f�[�^�B
		* @return DirectX::XMMATRIX �v���W�F�N�V�����s��B
		*/
		DirectX::XMMATRIX SBindCamera::CalcProjectionMatrix(component::CCamera _camera)
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

			return DirectX::XMLoadFloat4x4(&projMat);
		}

		/**
		* @brief �r���[�s���萔�o�b�t�@�ɏ������݂܂��B
		* @param _viewMat �r���[�s��B
		*/
		void SBindCamera::CBufWriteViewMatrix(DirectX::XMMATRIX _viewMat)
		{
			DirectX::XMFLOAT4X4 viewMat;

			DirectX::XMStoreFloat4x4(&viewMat, _viewMat);

			graphics::Graphics::GetInstance().GetRenderContext()
				->SetViewMatrix(&viewMat);
		}

		/**
		* @brief �v���W�F�N�V�����s���萔�o�b�t�@�ɏ������݂܂��B
		* @param _projMat �v���W�F�N�V�����s��B
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