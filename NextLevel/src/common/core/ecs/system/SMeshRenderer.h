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
			* @brief メッシュの描画を行うクラス。
			*/
			class SMeshRenderer : public SystemBase {
			public:
				/**
				* @brief コンストラクタ。
				*/
				SMeshRenderer(World* _pWorld)
					: SystemBase(_pWorld)
				{}

				/**
				* @brief 初期化処理をします。
				*/
				void Init() override;

				/**
				* @brief 描画処理をします。
				*/
				void Draw() override;

			private:
				/**
				* @brief アーキタイプに必要なコンポーネントを設定します。
				*/
				void ArchetypeSetting();

				/**
				* @brief Transformからワールド行列を算出します。
				* @param _transform 形状データ。
				* @return DirectX::XMFLOAT4X4 ワールド行列。
				*/
				DirectX::XMFLOAT4X4 CalcWorldMatrix(component::CTransform _transform);

				/**
				* @brief マテリアルIDからマテリアル情報を取得します。
				* @param _nMaterialId マテリアルID。
				* @return resource::MaterialData マテリアル情報。
				*/
				resource::MaterialData GetMaterialData(int _nMaterialId);

				/**
				* @brief ワールド行列を定数バッファに描き込みます。
				* @param _worldMat ワールド行列。
				*/
				void CBufWriteWorldMatrix(DirectX::XMFLOAT4X4 _worldMat);

				/**
				* @brief マテリアルデータを定数バッファに描き込みます。
				* @param _materialData マテリアル情報。
				*/
				void CBufWriteMaterialData(resource::MaterialData _materialData);
			
				/**
				* @brief 頂点シェーダーをバインドします。
				* @param _nVertexShaderId シェーダーID。
				*/
				void BindVertexShader(int _nVertexShaderId);

				/**
				* @brief ピクセルシェーダーをバインドします。
				* @param _nPixelShaderId シェーダーID。
				*/
				void BindPixelShader(int _nPixelShaderId);

				/**
				* @brief メッシュを描画します。
				* @param _nModelId モデルID。
				*/
				void DrawMesh(int _nModelId);
			};
		}
	}
}

#endif //!___SRC_COMMON_CORE_ECS_SYSTEM_SMESHRENDERER_H___