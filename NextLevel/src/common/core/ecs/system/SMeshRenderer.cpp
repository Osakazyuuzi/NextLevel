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
			* @brief 初期化処理をします。
			*/
			void SMeshRenderer::Init()
			{
				// アーキタイプに必要なコンポーネントを設定する。
				ArchetypeSetting();
			}

			/**
			* @brief 描画処理をします。
			*/
			void SMeshRenderer::Draw()
			{
				// 該当のエンティティが持つ該当のコンポーネントのみに
				// 受け渡す関数を適用する。
				this->ExecuteForEntitiesMatching
					<CTransform, CMeshRenderer>
					([this](CTransform& _transform, CMeshRenderer& _meshRenderer)
						{
							//=== 定数バッファの書き込み
							// ワールドマトリクスの書き込み
							CBufWriteWorldMatrix(
								CalcWorldMatrix(_transform));
							// マテリアルの書き込み
							CBufWriteMaterialData(
								GetMaterialData(_meshRenderer.m_materialId));
						
							//=== シェーダーのバインド
							// 頂点シェーダーのバインド
							BindVertexShader(_meshRenderer.m_vertexShaderId);
							// ピクセルシェーダーのバインド
							BindPixelShader(_meshRenderer.m_pixelShaderId);

							//=== メッシュの描画
							DrawMesh(_meshRenderer.m_modelId);
						});
			}

			/**
			* @brief アーキタイプに必要なコンポーネントを設定します。
			*/
			void SMeshRenderer::ArchetypeSetting()
			{
				// WorldMatrixを作成するのに必要。
				m_Archetype.AddType<CTransform>();

				// モデル情報やマテリアル情報など、描画するには必要不可欠。
				m_Archetype.AddType<CMeshRenderer>();
			}

			/**
			* @brief Transformからワールド行列を算出します。
			* @param _transform 形状データ。
			* @return DirectX::XMFLOAT4X4 ワールド行列。
			*/
			DirectX::XMFLOAT4X4 SMeshRenderer::CalcWorldMatrix(component::CTransform _transform)
			{
				DirectX::XMFLOAT4X4 worldMat;
				DirectX::XMMATRIX posMat, rotMat, scaleMat;

				// 座標の行列を求める
				posMat = DirectX::XMMatrixTranslation(
					_transform.m_position.x(),
					_transform.m_position.y(),
					_transform.m_position.z()
				);
				// 回転の行列を求める
				rotMat = DirectX::XMMatrixRotationRollPitchYaw(
					_transform.m_rotation.x(),
					_transform.m_rotation.y(), 
					_transform.m_rotation.z()
				);
				// 拡縮の行列を求める
				scaleMat = DirectX::XMMatrixScaling(
					_transform.m_scale.x(),
					_transform.m_scale.y(),
					_transform.m_scale.z()
				);
				// 行列を掛け合わせ、FLOAT4X4に変換する
				DirectX::XMStoreFloat4x4(&worldMat,
					scaleMat * rotMat * posMat);

				return worldMat;
			}

			/**
			* @brief マテリアルIDからマテリアル情報を取得します。
			* @param _nMaterialId マテリアルID。
			* @return resource::MaterialData マテリアル情報。
			*/
			resource::MaterialData SMeshRenderer::GetMaterialData(int _nMaterialId)
			{
				return *(resource::MaterialManager::GetInstance().GetMaterialData(_nMaterialId).value().get());
			}

			/**
			* @brief ワールド行列を定数バッファに描き込みます。
			* @param _worldMat ワールド行列。
			*/
			void SMeshRenderer::CBufWriteWorldMatrix(DirectX::XMFLOAT4X4 _worldMat)
			{
				graphics::Graphics::GetInstance().GetRenderContext()->SetWorldMatrix(&_worldMat);
			}

			/**
			* @brief マテリアルデータを定数バッファに描き込みます。
			* @param _materialData マテリアル情報。
			*/
			void SMeshRenderer::CBufWriteMaterialData(resource::MaterialData _materialData)
			{
				graphics::Graphics::GetInstance().GetRenderContext()->SetMaterial(_materialData);
			}

			/**
			* @brief 頂点シェーダーをバインドします。
			* @param _nVertexShaderId シェーダーID。
			*/
			void SMeshRenderer::BindVertexShader(int _nVertexShaderId)
			{
				resource::ShaderManager::GetInstance().GetShaderData(_nVertexShaderId).value()->Bind();
			}

			/**
			* @brief ピクセルシェーダーをバインドします。
			* @param _nPixelShaderId シェーダーID。
			*/
			void SMeshRenderer::BindPixelShader(int _nPixelShaderId)
			{
				resource::ShaderManager::GetInstance().GetShaderData(_nPixelShaderId).value()->Bind();
			}

			/**
			* @brief メッシュを描画します。
			* @param _nModelId モデルID。
			*/
			void SMeshRenderer::DrawMesh(int _nModelId)
			{
				resource::ModelManager::GetInstance().GetModelData(_nModelId).value()->Draw();
			}
		}
	}
}