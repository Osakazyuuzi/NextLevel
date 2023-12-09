#include "Graphics.h"

#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include <array>
#include "low_level/RenderTarget.h"
#include "low_level/DepthStencil.h"
#include "low_level/RenderContext.h"

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

namespace NextLevel
{
	namespace graphics
	{
		/**
		* @brief �O���t�B�b�N�X�V�X�e���̏����������܂��B
		* @param _hWnd �E�B���h�E�n���h�B
		* @param _width �E�B���h�E�̉����B
		* @param _height �E�B���h�E�̍����B
		* @return bool �����������������ꍇ��true,���s�����ꍇ��false�B
		*/
		bool Graphics::Startup(HWND _hWnd, UINT _width, UINT _height)
		{
			m_Width = _width;
			m_Height = _height;

			HRESULT	hr = E_FAIL;
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));						// �[���N���A
			sd.BufferDesc.Width = _width;							// �o�b�N�o�b�t�@�̕�
			sd.BufferDesc.Height = _height;						// �o�b�N�o�b�t�@�̍���
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B,A)
			sd.SampleDesc.Count = 1;							// �}���`�T���v���̐�
			sd.BufferDesc.RefreshRate.Numerator = 1000;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@
			sd.BufferCount = 1;									// �o�b�N�o�b�t�@�̐�
			sd.OutputWindow = _hWnd;							// �֘A�t����E�C���h�E
			sd.Windowed = TRUE;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			// �h���C�o�̎��
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,	// GPU�ŕ`��
				D3D_DRIVER_TYPE_WARP,		// �����x(�ᑬ
				D3D_DRIVER_TYPE_REFERENCE,	// CPU�ŕ`��
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			UINT createDeviceFlags = 0;
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

			// �@�\���x��
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
				D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
				D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
				D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
				D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
				D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
				D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			D3D_DRIVER_TYPE driverType;
			D3D_FEATURE_LEVEL featureLevel;

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
			{
				driverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(
					NULL,					// �f�B�X�v���C�f�o�C�X�̃A�_�v�^�iNULL�̏ꍇ�ŏ��Ɍ��������A�_�v�^�j
					driverType,				// �f�o�C�X�h���C�o�̃^�C�v
					NULL,					// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw�肷��
					createDeviceFlags,		// �f�o�C�X�t���O
					featureLevels,			// �@�\���x��
					numFeatureLevels,		// �@�\���x����
					D3D11_SDK_VERSION,		// 
					&sd,					// �X���b�v�`�F�C���̐ݒ�
					&m_pSwapChain,			// IDXGIDwapChain�C���^�t�F�[�X	
					&m_pDevice,				// ID3D11Device�C���^�t�F�[�X
					&featureLevel,		// �T�|�[�g����Ă���@�\���x��
					&m_pContext);		// �f�o�C�X�R���e�L�X�g
				if (SUCCEEDED(hr)) {
					break;
				}
			}
			if (FAILED(hr)) {
				return false;
			}

			// �t���[���o�b�t�@�p�̃����_�[�^�[�Q�b�g���쐬����
			m_pRenderTarget = std::make_shared<lowlevel::RenderTarget>();
			m_pRenderTarget->CreateFromScreen();

			// �t���[���o�b�t�@�p�̃f�v�X�X�e���V�����쐬����
			m_pDepthStencil = std::make_shared<lowlevel::DepthStencil>();
			m_pDepthStencil->Create(m_Width, m_Height, false);

			// �����_�[�R���e�L�X�g�̏�����������
			m_renderContext = std::make_shared<lowlevel::RenderContext>();
			m_renderContext->Init();

			return true;
		}

		/**
		* @brief �O���t�B�b�N�X�V�X�e���̏I�����������܂��B
		*/
		void Graphics::Shutdown()
		{
			// �����_�[�R���e�L�X�g�̏I������������
			m_renderContext->Uninit();

			m_pContext->ClearState();
			m_pSwapChain->SetFullscreenState(false, NULL);
		}

		/**
		* @brief �����_�����O�̊J�n���������܂��B
		* @param _red �w�i�F�̐Ԑ���
		* @param _green �w�i�F�̗ΐ���
		* @param _blue �w�i�F�̐���
		*/
		void Graphics::BeginRender(float _red, float _green, float _blue)
		{
			m_renderContext->SetRenderTargets(1, &m_pRenderTarget, m_pDepthStencil);

			m_renderContext->ClearRenderTargetView(m_pRenderTarget, { _red, _green, _blue, 1.0f });
			m_renderContext->ClearDepthStencilView(m_pDepthStencil);
		}

		/**
		* @brief �����_�����O�̏I�����������܂��B
		*/
		void Graphics::EndRender()
		{
			m_pSwapChain->Present(0, 0);
		}
	}
}