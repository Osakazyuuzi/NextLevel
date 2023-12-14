#include "ShaderData.h"

#include <d3dcompiler.h>
#include <cstdio>
#include <io.h>

#include "src/engine/graphics/windows/Graphics.h"

#pragma comment(lib, "d3dcompiler.lib")

namespace NextLevel
{
	namespace resource
	{
		/**
		* @brief �V�F�[�_�[�����[�h���܂��B
		* @param _kind �V�F�[�_�[�̎�ށB
		* @param _fileName �V�F�[�_�[�t�@�C�����B
		* @return bool ���[�h����������true�B
		*/
		bool ShaderData::Load(EnKind _kind, const char* _fileName)
		{
			// ��ޔ���
			if (_kind < 0 || EnKind::enMax <= _kind) return false;
			// ���ʎq����
			if (!strstr(_fileName, ".cso")) return false;
			// ���ݔ���
			struct stat buffer;
			if (stat(_fileName, &buffer) != 0) return false;

			m_szFileName = _fileName;
			m_enKind = _kind;

			// �ǂݍ���
			switch (m_enKind)
			{
			case EnKind::enVertex:
			{
				FILE* file;
				long int fsize;

				file = fopen(_fileName, "rb");
				if (!file) {
					return false;
				}

				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				graphics::Graphics::GetInstance().GetDevice()->CreateVertexShader(buffer, fsize, NULL, &m_pVS);


				D3D11_INPUT_ELEMENT_DESC layout[] =
				{
					{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
				};
				UINT numElements = ARRAYSIZE(layout);

				graphics::Graphics::GetInstance().GetDevice()->CreateInputLayout(layout,
					numElements,
					buffer,
					fsize,
					&m_pInputLayout);

				delete[] buffer;

				break;
			}
			case EnKind::enPixel:
			{
				FILE* file;
				long int fsize;

				file = fopen(_fileName, "rb");
				if (!file) {
					return false;
				}

				fsize = _filelength(_fileno(file));
				unsigned char* buffer = new unsigned char[fsize];
				fread(buffer, fsize, 1, file);
				fclose(file);

				graphics::Graphics::GetInstance().GetDevice()->CreatePixelShader(buffer, fsize, NULL, &m_pPS);

				delete[] buffer;

				break;
			}
			}
			return true;
		}

		/**
		* @brief �V�F�[�_�[���o�C���h���܂��B
		*/
		void ShaderData::Bind()
		{
			switch (m_enKind)
			{
			case EnKind::enVertex:
			{
				// ���̓��C�A�E�g��ݒ�
				graphics::Graphics::GetInstance().GetDeviceContext()->IASetInputLayout(m_pInputLayout.Get());

				// �V�F�[�_�[��ݒ�
				graphics::Graphics::GetInstance().GetDeviceContext()->VSSetShader(m_pVS.Get(), nullptr, 0);

				break;
			}
			case EnKind::enPixel:
			{
				// �V�F�[�_�[��ݒ�
				graphics::Graphics::GetInstance().GetDeviceContext()->PSSetShader(m_pPS.Get(), nullptr, 0);

				break;
			}
			}
		}
	}
}