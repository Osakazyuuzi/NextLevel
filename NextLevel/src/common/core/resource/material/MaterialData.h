#ifndef ___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALDATA_H___

#include <DirectXMath.h>
#include <d3d11.h>

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class MaterialData
		* @brief マテリアルデータを管理するクラス。
		*/
		class MaterialData {
		public:
			//! 環境光の色。
			DirectX::XMFLOAT4 m_Ambient;
			//! 拡散反射の色。
			DirectX::XMFLOAT4 m_Diffuse;
			//! 鏡面反射の色。
			DirectX::XMFLOAT4 m_Specular;
			//! 自己発光の色。
			DirectX::XMFLOAT4 m_Emission;
			//! 光沢度。
			float m_Shininess;
			//! テクスチャの使用有無。
			BOOL m_TextureEnable;
			//! パディング。
			float Dummy[2];
		};
	}
}

inline bool operator==(
	const NextLevel::resource::MaterialData& _left,
	const NextLevel::resource::MaterialData& _right)
{
	return _left.m_Ambient.x == _right.m_Ambient.x
		&& _left.m_Ambient.y == _right.m_Ambient.y
		&& _left.m_Ambient.z == _right.m_Ambient.z
		&& _left.m_Ambient.w == _right.m_Ambient.w
		&& _left.m_Diffuse.x == _right.m_Diffuse.x
		&& _left.m_Diffuse.y == _right.m_Diffuse.y
		&& _left.m_Diffuse.z == _right.m_Diffuse.z
		&& _left.m_Diffuse.w == _right.m_Diffuse.w
		&& _left.m_Specular.x == _right.m_Specular.x
		&& _left.m_Specular.y == _right.m_Specular.y
		&& _left.m_Specular.z == _right.m_Specular.z
		&& _left.m_Specular.w == _right.m_Specular.w
		&& _left.m_Emission.x == _right.m_Emission.x
		&& _left.m_Emission.y == _right.m_Emission.y
		&& _left.m_Emission.z == _right.m_Emission.z
		&& _left.m_Emission.w == _right.m_Emission.w
		&& _left.m_Shininess == _right.m_Shininess
		&& _left.m_TextureEnable == _right.m_TextureEnable;
}


#endif //!___SRC_COMMON_CORE_RESOURCE_MATERIAL_MATERIALDATA_H___