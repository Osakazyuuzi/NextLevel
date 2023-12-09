#ifndef ___SRC_GRAPHICS_GRAPHICS_H___
#define ___SRC_GRAPHICS_GRAPHICS_H___

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <memory>
#include <wrl.h>

#include "low_level/RenderTarget.h"
#include "low_level/DepthStencil.h"
#include "low_level/RenderContext.h"

namespace NextLevel
{
	namespace graphics
	{
		/**
		* @class Graphics
		* @brief �O���t�B�b�N�X�֘A�̑�����Ǘ�����N���X�B
		*/
		class Graphics
		{
		private:
			/**
			* @brief �R���X�g���N�^�B
			*/
			Graphics() = default;

			/**
			* @brief �f�X�g���N�^�B
			*/
			~Graphics() = default;

		public:
			/**
			 * @brief �V���O���g���C���X�^���X���擾���܂��B
			 * @return Graphics& �O���t�B�b�N�X�N���X�̃C���X�^���X�B
			 */
			static Graphics& GetInstance()
			{
				static Graphics instance;
				return instance;
			}

			/**
			* @brief �O���t�B�b�N�X�V�X�e���̏����������܂��B
			* @param _hWnd �E�B���h�E�n���h�B
			* @param _width �E�B���h�E�̉����B
			* @param _height �E�B���h�E�̍����B
			* @return bool �����������������ꍇ��true,���s�����ꍇ��false�B
			*/
			bool Startup(HWND _hWnd, UINT _width, UINT _height);

			/**
			* @brief �O���t�B�b�N�X�V�X�e���̏I�����������܂��B
			*/
			void Shutdown();

			/**
			* @brief �����_�����O�̊J�n���������܂��B
			* @param _red �w�i�F�̐Ԑ���
			* @param _green �w�i�F�̗ΐ���
			* @param _blue �w�i�F�̐���
			*/
			void BeginRender(float _red = 1.0f, float _green = 1.0f, float _blue = 1.0f);

			/**
			* @brief �����_�����O�̏I�����������܂��B
			*/
			void EndRender();

			/**
			* @brief �f�o�C�X�̃|�C���^���擾���܂��B
			* @return Microsoft::WRL::ComPtr<ID3D11Device> �f�o�C�X�̃|�C���^�B
			*/
			Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const noexcept
			{
				return m_pDevice;
			}

			/**
			* @brief �f�o�C�X�R���e�L�X�g�̃|�C���^���擾���܂��B
			* @return Microsoft::WRL::ComPtr<ID3D11DeviceContext> �f�o�C�X�R���e�L�X�g�̃|�C���^�B
			*/
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const noexcept
			{
				return m_pContext;
			}

			/**
			* @brief �X���b�v�`�F�[���̃|�C���^���擾���܂��B
			* @return Microsoft::WRL::ComPtr<IDXGISwapChain> �X���b�v�`�F�[���̃|�C���^�B
			*/
			Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const noexcept
			{
				return m_pSwapChain;
			}

			/**
			* @brief �����_�����O�R���e�L�X�g�̃|�C���^���擾���܂��B
			* @return std::shared_ptr<lowlevel::RenderContext> �����_�����O�R���e�L�X�g�̃|�C���^�B
			*/
			std::shared_ptr<lowlevel::RenderContext> GetRenderContext() const noexcept
			{
				return m_renderContext;
			}

			/**
			* @brief �E�B���h�E�̉������擾���܂��B
			* @return UINT �E�B���h�E�̉����B
			*/
			const UINT GetWidth() const noexcept
			{
				return m_Width;
			}

			/**
			* @brief �E�B���h�E�̏c�����擾���܂��B
			* @return UINT �E�B���h�E�̏c���B
			*/
			const UINT GetHeight() const noexcept
			{
				return m_Height;
			}

		private:
			//! DirectX11 �f�o�C�X�B
			Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
			//! DirectX11 �f�o�C�X�R���e�L�X�g�B
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;
			//! �X���b�v�`�F�[���B
			Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
			//! �f�t�H���g�����_�[�^�[�Q�b�g�B
			std::shared_ptr<lowlevel::RenderTarget> m_pRenderTarget = nullptr;
			//! �f�t�H���g�f�v�X�X�e���V���B
			std::shared_ptr<lowlevel::DepthStencil> m_pDepthStencil = nullptr;
			//! �����_�����O�R���e�L�X�g�B
			std::shared_ptr<lowlevel::RenderContext> m_renderContext = nullptr;
			//! �E�B���h�E�̉����B
			UINT m_Width = 0;
			//! �E�B���h�E�̏c���B
			UINT m_Height = 0;
		};
	}
}

#endif //!___SRC_CORE_GRAPHICS_GRAPHICS_H___