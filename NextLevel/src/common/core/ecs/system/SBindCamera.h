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
			* @brief �R���X�g���N�^�B
			* @param _pWorld ���[���h�|�C���^�B
			*/
			SBindCamera(World* _pWorld)
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
			* @param _transform �`��f�[�^�B
			* @return DirectX::XMMATRIX �r���[�s��B
			*/
			DirectX::XMMATRIX CalcViewMatrix(component::CTransform _transform);

			/**
			* @brief Camera����v���W�F�N�V�����s����Z�o���܂��B
			* @param _camera �J�����f�[�^�B
			* @return DirectX::XMMATRIX �v���W�F�N�V�����s��B
			*/
			DirectX::XMMATRIX CalcProjectionMatrix(component::CCamera _camera);
		
			/**
			* @brief �r���[�s���萔�o�b�t�@�ɏ������݂܂��B
			* @param _viewMat �r���[�s��B
			*/
			void CBufWriteProjectionMatrix(DirectX::XMMATRIX _projMat);

			/**
			* @brief �v���W�F�N�V�����s���萔�o�b�t�@�ɏ������݂܂��B
			* @param _projMat �v���W�F�N�V�����s��B
			*/
			void CBufWriteViewMatrix(DirectX::XMMATRIX _viewMat);
		};
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SBINDDEBUGCAMERA_H___