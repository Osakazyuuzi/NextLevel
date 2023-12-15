#ifndef ___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___

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
			class SBindDebugCamera : public SystemBase {
			public:
				/**
				* @brief �R���X�g���N�^�B
				* @param _pWorld ���[���h�|�C���^�B
				*/
				SBindDebugCamera(World* _pWorld)
					: SystemBase(_pWorld)
				{}

				/**
				* @brief ���������������܂��B
				*/
				void Init() override;

				/**
				* @brief �`�揈�������܂��B
				*/
				void Draw() override;

			private:
				/**
				* @brief �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肵�܂��B
				*/
				void ArchetypeSetting();

				/**
				* @brief Camera����r���[�s����Z�o���܂��B
				* @param _camera �J�����f�[�^�B
				* @return DirectX::XMFLOAT4X4 �r���[�s��B
				*/
				DirectX::XMFLOAT4X4 CalcViewMatrix(component::CDebugCamera _camera);

				/**
				* @brief Camera����v���W�F�N�V�����s����Z�o���܂��B
				* @param _camera �J�����f�[�^�B
				* @return DirectX::XMFLOAT4X4 �v���W�F�N�V�����s��B
				*/
				DirectX::XMFLOAT4X4 CalcProjectionMatrix(component::CDebugCamera _camera);

				/**
				* @brief �r���[�s���萔�o�b�t�@�ɏ������݂܂��B
				* @param _viewMat �r���[�s��B
				*/
				void CBufWriteProjectionMatrix(DirectX::XMFLOAT4X4* _projMat);

				/**
				* @brief �v���W�F�N�V�����s���萔�o�b�t�@�ɏ������݂܂��B
				* @param _projMat �v���W�F�N�V�����s��B
				*/
				void CBufWriteViewMatrix(DirectX::XMFLOAT4X4* _viewMat);
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___