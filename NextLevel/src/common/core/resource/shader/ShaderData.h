#ifndef ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H___

#include <d3d11.h>
#include <string>
#include <wrl.h>

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class ShaderData
		* @brief �V�F�[�_�[�f�[�^���Ǘ�����N���X�B
		*/
		class ShaderData {
		public:
			/**
			* @brief �V�F�[�_�[�̎�ނ�񋓁B
			*/
			enum EnKind {
				enVertex,	// VertexShader
				enPixel,	// PixelShader
				enMax
			};

			/**
			* @brief �V�F�[�_�[�����[�h���܂��B
			* @param _kind �V�F�[�_�[�̎�ށB
			* @param _fileName �V�F�[�_�[�t�@�C�����B
			* @return bool ���[�h����������true�B
			*/
			bool Load(EnKind _kind, const char* _fileName);

			/**
			* @brief �V�F�[�_�[���o�C���h���܂��B
			*/
			void Bind();

			/**
			* @brief �V�F�[�_�[�̎�ނ��擾���܂��B
			* @return EnKind �V�F�[�_�[�̎�ށB
			*/
			const EnKind GetKind() const noexcept {
				return m_enKind;
			}

			/**
			* @brief �V�F�[�_�[�t�@�C�������擾���܂��B
			* @return std::string �V�F�[�_�[�t�@�C�����B
			*/
			const std::string GetFileName() const noexcept {
				return m_szFileName;
			}

		private:
			//! �V�F�[�_�[�̎�ށB
			EnKind m_enKind;
			//! ���_�V�F�[�_�[�f�[�^�B
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
			//! �C���v�b�g���C�A�E�g�B
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
			//! �s�N�Z���V�F�[�_�[�f�[�^�B
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
			//! �V�F�[�_�[�t�@�C�����B
			std::string m_szFileName;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE_SHADER_SHADERDATA_H____