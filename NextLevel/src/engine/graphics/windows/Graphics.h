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
		* @brief グラフィックス関連の操作を管理するクラス。
		*/
		class Graphics
		{
		private:
			/**
			* @brief コンストラクタ。
			*/
			Graphics() = default;

			/**
			* @brief デストラクタ。
			*/
			~Graphics() = default;

		public:
			/**
			 * @brief シングルトンインスタンスを取得します。
			 * @return Graphics& グラフィックスクラスのインスタンス。
			 */
			static Graphics& GetInstance()
			{
				static Graphics instance;
				return instance;
			}

			/**
			* @brief グラフィックスシステムの初期化をします。
			* @param _hWnd ウィンドウハンドつ。
			* @param _width ウィンドウの横幅。
			* @param _height ウィンドウの高さ。
			* @return bool 初期化が成功した場合はtrue,失敗した場合はfalse。
			*/
			bool Startup(HWND _hWnd, UINT _width, UINT _height);

			/**
			* @brief グラフィックスシステムの終了処理をします。
			*/
			void Shutdown();

			/**
			* @brief レンダリングの開始処理をします。
			* @param _red 背景色の赤成分
			* @param _green 背景色の緑成分
			* @param _blue 背景色の青成分
			*/
			void BeginRender(float _red = 1.0f, float _green = 1.0f, float _blue = 1.0f);

			/**
			* @brief レンダリングの終了処理をします。
			*/
			void EndRender();

			/**
			* @brief デバイスのポインタを取得します。
			* @return Microsoft::WRL::ComPtr<ID3D11Device> デバイスのポインタ。
			*/
			Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const noexcept
			{
				return m_pDevice;
			}

			/**
			* @brief デバイスコンテキストのポインタを取得します。
			* @return Microsoft::WRL::ComPtr<ID3D11DeviceContext> デバイスコンテキストのポインタ。
			*/
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const noexcept
			{
				return m_pContext;
			}

			/**
			* @brief スワップチェーンのポインタを取得します。
			* @return Microsoft::WRL::ComPtr<IDXGISwapChain> スワップチェーンのポインタ。
			*/
			Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const noexcept
			{
				return m_pSwapChain;
			}

			/**
			* @brief レンダリングコンテキストのポインタを取得します。
			* @return std::shared_ptr<lowlevel::RenderContext> レンダリングコンテキストのポインタ。
			*/
			std::shared_ptr<lowlevel::RenderContext> GetRenderContext() const noexcept
			{
				return m_renderContext;
			}

			/**
			* @brief ウィンドウの横幅を取得します。
			* @return UINT ウィンドウの横幅。
			*/
			const UINT GetWidth() const noexcept
			{
				return m_Width;
			}

			/**
			* @brief ウィンドウの縦幅を取得します。
			* @return UINT ウィンドウの縦幅。
			*/
			const UINT GetHeight() const noexcept
			{
				return m_Height;
			}

		private:
			//! DirectX11 デバイス。
			Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice = nullptr;
			//! DirectX11 デバイスコンテキスト。
			Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext = nullptr;
			//! スワップチェーン。
			Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
			//! デフォルトレンダーターゲット。
			std::shared_ptr<lowlevel::RenderTarget> m_pRenderTarget = nullptr;
			//! デフォルトデプスステンシル。
			std::shared_ptr<lowlevel::DepthStencil> m_pDepthStencil = nullptr;
			//! レンダリングコンテキスト。
			std::shared_ptr<lowlevel::RenderContext> m_renderContext = nullptr;
			//! ウィンドウの横幅。
			UINT m_Width = 0;
			//! ウィンドウの縦幅。
			UINT m_Height = 0;
		};
	}
}

#endif //!___SRC_CORE_GRAPHICS_GRAPHICS_H___