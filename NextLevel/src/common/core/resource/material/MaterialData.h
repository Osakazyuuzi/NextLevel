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
		* @brief �}�e���A���f�[�^���Ǘ�����N���X�B
		*/
		class MaterialData {
		public:
			//! �����̐F�B
			DirectX::XMFLOAT4 m_Ambient;
			//! �g�U���˂̐F�B
			DirectX::XMFLOAT4 m_Diffuse;
			//! ���ʔ��˂̐F�B
			DirectX::XMFLOAT4 m_Specular;
			//! ���Ȕ����̐F�B
			DirectX::XMFLOAT4 m_Emission;
			//! ����x�B
			float m_Shininess;
			//! �e�N�X�`���̎g�p�L���B
			BOOL m_TextureEnable;
			//! �p�f�B���O�B
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