#ifndef ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREDATA_H___

#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class TextureData
		* @brief �e�N�X�`���f�[�^���Ǘ�����N���X�B
		*/
		class TextureData
		{
		public:
			/**
			* @brief �R���X�g���N�^�B
			*/
			TextureData();

			/**
			* @brief �f�X�g���N�^�B
			*/
			virtual ~TextureData();

			/**
			 * @brief �t�@�C������e�N�X�`�����쐬���܂��B
			 * @param _fileName �e�N�X�`���t�@�C���̖��O�B
			 * @return HRESULT ����̌��ʁB
			 */
			HRESULT Create(const char* _fileName);

			/**
			 * @brief ����������e�N�X�`�����쐬���܂��B
			 * @param _format �e�N�X�`���̃t�H�[�}�b�g�B
			 * @param _width �e�N�X�`���̕��B
			 * @param _height �e�N�X�`���̍����B
			 * @param _pData �e�N�X�`���f�[�^�ւ̃|�C���^�B
			 * @return HRESULT ����̌��ʁB
			 */
			HRESULT Create(DXGI_FORMAT _format, UINT _width, UINT _height, const void* _pData = nullptr);

			/**
			* @brief �e�N�X�`���̉������擾���܂��B
			* @return UINT �e�N�X�`���̉����B
			*/
			const UINT GetWidth() const noexcept
			{
				return m_width;
			}

			/**
			* @brief �e�N�X�`���̏c�����擾���܂��B
			* @return UINT �e�N�X�`���̏c���B
			*/
			const UINT GetHeight() const noexcept
			{
				return m_height;
			}

			/**
			 * @brief �V�F�[�_�[���\�[�X�r���[�̃|�C���^���擾���܂��A
			 * @return Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> �V�F�[�_�[���\�[�X�r���[�̃|�C���^�B
			 */
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResource() const
			{
				return m_pSRV;
			}

		protected:
			/**
			 * @brief �e�N�X�`���L�q�q���쐬���܂��B
			 * @param _format �e�N�X�`���̃t�H�[�}�b�g�B
			 * @param _width �e�N�X�`���̕����B
			 * @param _height �e�N�X�`���̏c���B
			 * @return D3D11_TEXTURE2D_DESC �e�N�X�`�����B
			 */
			D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT _format, UINT _width, UINT _height);

			/**
			 * @brief ���\�[�X���쐬���܂��B
			 * @param _desc �e�N�X�`�����B
			 * @param _pData �e�N�X�`���f�[�^�ւ̃|�C���^�B
			 * @return HRESULT ����̌��ʁB
			 */
			virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData);

		protected:
			//! �e�N�X�`���̉����B
			UINT m_width;
			//! �e�N�X�`���̏c���B
			UINT m_height;
			//! DirectX11 �V�F�[�_�[���\�[�X�r���[�B
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
			//! DirectX11 �e�N�X�`���o�b�t�@�B
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTex;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE TEXTURE_TEXTUREDATA_H___