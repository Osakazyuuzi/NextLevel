#include "TextureData.h"

#include <iostream>

#include "src/engine/graphics/windows/Graphics.h"
#include "third_party/DirectXTex/TextureLoad.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief �R���X�g���N�^�B
		*/
		TextureData::TextureData()
			: m_width(0), m_height(0)
			, m_pTex(nullptr)
			, m_pSRV(nullptr)
		{}

		/**
		* @brief �f�X�g���N�^�B
		*/
		TextureData::~TextureData()
		{}

		/**
		 * @brief �t�@�C������e�N�X�`�����쐬���܂��B
		 * @param _fileName �e�N�X�`���t�@�C���̖��O�B
		 * @return HRESULT ����̌��ʁB
		 */
		HRESULT TextureData::Create(const char* _fileName)
		{
			HRESULT hr = S_OK;

			// �����ϊ�
			wchar_t wPath[MAX_PATH];
			size_t wLen = 0;
			MultiByteToWideChar(0, 0, _fileName, -1, wPath, MAX_PATH);

			// �t�@�C���ʓǂݍ���
			DirectX::TexMetadata mdata;
			DirectX::ScratchImage image;
			if (strstr(_fileName, ".tga"))
				hr = DirectX::LoadFromTGAFile(wPath, &mdata, image);
			else
				hr = DirectX::LoadFromWICFile(wPath, DirectX::WIC_FLAGS::WIC_FLAGS_NONE, &mdata, image);
			if (FAILED(hr)) {
				return E_FAIL;
			}

			// �V�F�[�_���\�[�X����
			hr = CreateShaderResourceView(graphics::Graphics::GetInstance().GetDevice().Get(), image.GetImages(), image.GetImageCount(), mdata, &m_pSRV);
			if (SUCCEEDED(hr))
			{
				m_width = (UINT)mdata.width;
				m_height = (UINT)mdata.height;
			}
			return hr;
		}

		/**
		 * @brief ����������e�N�X�`�����쐬���܂��B
		 * @param _format �e�N�X�`���̃t�H�[�}�b�g�B
		 * @param _width �e�N�X�`���̕��B
		 * @param _height �e�N�X�`���̍����B
		 * @param _pData �e�N�X�`���f�[�^�ւ̃|�C���^�B
		 * @return HRESULT ����̌��ʁB
		 */
		HRESULT TextureData::Create(DXGI_FORMAT _format, UINT _width, UINT _height, const void* _pData)
		{
			D3D11_TEXTURE2D_DESC desc = MakeTexDesc(_format, _width, _height);
			return CreateResource(desc, _pData);
		}

		/**
		 * @brief �e�N�X�`���L�q�q���쐬���܂��B
		 * @param _format �e�N�X�`���̃t�H�[�}�b�g�B
		 * @param _width �e�N�X�`���̕����B
		 * @param _height �e�N�X�`���̏c���B
		 * @return D3D11_TEXTURE2D_DESC �e�N�X�`�����B
		 */
		D3D11_TEXTURE2D_DESC TextureData::MakeTexDesc(DXGI_FORMAT _format, UINT _width, UINT _height)
		{
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.Format = _format;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.Width = _width;
			desc.Height = _height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.SampleDesc.Count = 1;
			return desc;
		}

		/**
		 * @brief ���\�[�X���쐬���܂��B
		 * @param _desc �e�N�X�`�����B
		 * @param _pData �e�N�X�`���f�[�^�ւ̃|�C���^�B
		 * @return HRESULT ����̌��ʁB
		 */
		HRESULT TextureData::CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData)
		{
			HRESULT hr = E_FAIL;

			// �e�N�X�`���쐬
			D3D11_SUBRESOURCE_DATA data = {};
			data.pSysMem = _pData;
			data.SysMemPitch = _desc.Width * 4;
			hr = graphics::Graphics::GetInstance().GetDevice()->CreateTexture2D(&_desc, _pData ? &data : nullptr, &m_pTex);
			if (FAILED(hr)) { return hr; }

			// �ݒ�
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
			switch (_desc.Format)
			{
			default:						srvDesc.Format = _desc.Format;			break;
			case DXGI_FORMAT_R32_TYPELESS: 	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;	break;
			}
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = 1;
			// ����
			hr = graphics::Graphics::GetInstance().GetDevice()->CreateShaderResourceView(m_pTex.Get(), &srvDesc, &m_pSRV);
			if (SUCCEEDED(hr))
			{
				m_width = _desc.Width;
				m_height = _desc.Height;
			}
			return hr;
		}
	}
}