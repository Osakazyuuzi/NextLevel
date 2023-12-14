#include "SMeshRenderer.h"

#include "../World.h"
#include "../EntityManager.h"

#include "../component/CTransform.h"
#include "../component/CMeshRenderer.h"

#include "src/engine/graphics/windows/Graphics.h"
#include "src/common/core/resource/material/MaterialData.h"
#include "src/common/core/resource/material/MaterialManager.h"
#include "src/common/core/resource/shader/ShaderData.h"
#include "src/common/core/resource/shader/ShaderManager.h"
#include "src/common/core/resource/model/ModelData.h"
#include "src/common/core/resource/model/ModelManager.h"

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
			void SMeshRenderer::Init()
			{
				// �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肷��B
				ArchetypeSetting();
			}

			/**
			* @brief �`�揈�������܂��B
			*/
			void SMeshRenderer::Draw()
			{
				// �Y���̃G���e�B�e�B�����Y���̃R���|�[�l���g�݂̂�
				// �󂯓n���֐���K�p����B
				this->ExecuteForEntitiesMatching
					<CTransform, CMeshRenderer>
					([this](CTransform& _transform, CMeshRenderer& _meshRenderer)
						{
							//=== �萔�o�b�t�@�̏�������
							// ���[���h�}�g���N�X�̏�������
							CBufWriteWorldMatrix(
								CalcWorldMatrix(_transform));
							// �}�e���A���̏�������
							CBufWriteMaterialData(
								GetMaterialData(_meshRenderer.m_materialId));
						
							//=== �V�F�[�_�[�̃o�C���h
							// ���_�V�F�[�_�[�̃o�C���h
							BindVertexShader(_meshRenderer.m_vertexShaderId);
							// �s�N�Z���V�F�[�_�[�̃o�C���h
							BindPixelShader(_meshRenderer.m_pixelShaderId);

							//=== ���b�V���̕`��
							DrawMesh(_meshRenderer.m_modelId);
						});
			}

			/**
			* @brief �A�[�L�^�C�v�ɕK�v�ȃR���|�[�l���g��ݒ肵�܂��B
			*/
			void SMeshRenderer::ArchetypeSetting()
			{
				// WorldMatrix���쐬����̂ɕK�v�B
				m_Archetype.AddType<CTransform>();

				// ���f������}�e���A�����ȂǁA�`�悷��ɂ͕K�v�s���B
				m_Archetype.AddType<CMeshRenderer>();
			}

			/**
			* @brief Transform���烏�[���h�s����Z�o���܂��B
			* @param _transform �`��f�[�^�B
			* @return DirectX::XMFLOAT4X4 ���[���h�s��B
			*/
			DirectX::XMFLOAT4X4 SMeshRenderer::CalcWorldMatrix(component::CTransform _transform)
			{
				DirectX::XMFLOAT4X4 worldMat;
				DirectX::XMMATRIX posMat, rotMat, scaleMat;

				// ���W�̍s������߂�
				posMat = DirectX::XMMatrixTranslation(
					_transform.m_position.x(),
					_transform.m_position.y(),
					_transform.m_position.z()
				);
				// ��]�̍s������߂�
				rotMat = DirectX::XMMatrixRotationRollPitchYaw(
					_transform.m_rotation.x(),
					_transform.m_rotation.y(), 
					_transform.m_rotation.z()
				);
				// �g�k�̍s������߂�
				scaleMat = DirectX::XMMatrixScaling(
					_transform.m_scale.x(),
					_transform.m_scale.y(),
					_transform.m_scale.z()
				);
				// �s����|�����킹�AFLOAT4X4�ɕϊ�����
				DirectX::XMStoreFloat4x4(&worldMat,
					scaleMat * rotMat * posMat);

				return worldMat;
			}

			/**
			* @brief �}�e���A��ID����}�e���A�������擾���܂��B
			* @param _nMaterialId �}�e���A��ID�B
			* @return resource::MaterialData �}�e���A�����B
			*/
			resource::MaterialData SMeshRenderer::GetMaterialData(int _nMaterialId)
			{
				return *(resource::MaterialManager::GetInstance().GetMaterialData(_nMaterialId).value().get());
			}

			/**
			* @brief ���[���h�s���萔�o�b�t�@�ɕ`�����݂܂��B
			* @param _worldMat ���[���h�s��B
			*/
			void SMeshRenderer::CBufWriteWorldMatrix(DirectX::XMFLOAT4X4 _worldMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()->SetWorldMatrix(&_worldMat);
			}

			/**
			* @brief �}�e���A���f�[�^��萔�o�b�t�@�ɕ`�����݂܂��B
			* @param _materialData �}�e���A�����B
			*/
			void SMeshRenderer::CBufWriteMaterialData(resource::MaterialData _materialData)
			{
				graphics::Graphics::GetInstance().GetRenderContext()->SetMaterial(_materialData);
			}

			/**
			* @brief ���_�V�F�[�_�[���o�C���h���܂��B
			* @param _nVertexShaderId �V�F�[�_�[ID�B
			*/
			void SMeshRenderer::BindVertexShader(int _nVertexShaderId)
			{
				resource::ShaderManager::GetInstance().GetShaderData(_nVertexShaderId).value()->Bind();
			}

			/**
			* @brief �s�N�Z���V�F�[�_�[���o�C���h���܂��B
			* @param _nPixelShaderId �V�F�[�_�[ID�B
			*/
			void SMeshRenderer::BindPixelShader(int _nPixelShaderId)
			{
				resource::ShaderManager::GetInstance().GetShaderData(_nPixelShaderId).value()->Bind();
			}

			/**
			* @brief ���b�V����`�悵�܂��B
			* @param _nModelId ���f��ID�B
			*/
			void SMeshRenderer::DrawMesh(int _nModelId)
			{
				resource::ModelManager::GetInstance().GetModelData(_nModelId).value()->Draw();
			}
		}
	}
}