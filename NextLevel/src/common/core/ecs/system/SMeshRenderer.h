#ifndef ___SRC_COMMON_CORE_ECS_SYSTEM_SMESHRENDERER_H___
#define ___SRC_COMMON_CORE_ECS_SYSTEM_SMESHRENDERER_H___

#include "../SystemBase.h"

#include <DirectXMath.h>

namespace NextLevel
{
	namespace resource
	{
		class MaterialData;
	}

	namespace ecs
	{
		namespace component
		{
			class CTransform;
		}

		namespace system
		{
			/**
			* @class SMeshRenderer
			* @brief ���b�V���̕`����s���N���X�B
			*/
			class SMeshRenderer : public SystemBase {
			public:
				/**
				* @brief �R���X�g���N�^�B
				*/
				SMeshRenderer(World* _pWorld)
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
				* @brief Transform���烏�[���h�s����Z�o���܂��B
				* @param _transform �`��f�[�^�B
				* @return DirectX::XMFLOAT4X4 ���[���h�s��B
				*/
				DirectX::XMFLOAT4X4 CalcWorldMatrix(component::CTransform _transform);

				/**
				* @brief �}�e���A��ID����}�e���A�������擾���܂��B
				* @param _nMaterialId �}�e���A��ID�B
				* @return resource::MaterialData �}�e���A�����B
				*/
				resource::MaterialData GetMaterialData(int _nMaterialId);

				/**
				* @brief ���[���h�s���萔�o�b�t�@�ɕ`�����݂܂��B
				* @param _worldMat ���[���h�s��B
				*/
				void CBufWriteWorldMatrix(DirectX::XMFLOAT4X4 _worldMat);

				/**
				* @brief �}�e���A���f�[�^��萔�o�b�t�@�ɕ`�����݂܂��B
				* @param _materialData �}�e���A�����B
				*/
				void CBufWriteMaterialData(resource::MaterialData _materialData);
			
				/**
				* @brief ���_�V�F�[�_�[���o�C���h���܂��B
				* @param _nVertexShaderId �V�F�[�_�[ID�B
				*/
				void BindVertexShader(int _nVertexShaderId);

				/**
				* @brief �s�N�Z���V�F�[�_�[���o�C���h���܂��B
				* @param _nPixelShaderId �V�F�[�_�[ID�B
				*/
				void BindPixelShader(int _nPixelShaderId);

				/**
				* @brief ���b�V����`�悵�܂��B
				* @param _nModelId ���f��ID�B
				*/
				void DrawMesh(int _nModelId);
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SMESHRENDERER_H___