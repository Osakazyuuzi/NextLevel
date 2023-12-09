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
		* @brief グラフィックスシステムの初期化をします。
		* @param _hWnd ウィンドウハンドつ。
		* @param _width ウィンドウの横幅。
		* @param _height ウィンドウの高さ。
		* @return bool 初期化が成功した場合はtrue,失敗した場合はfalse。
		*/
		bool Graphics::Startup(HWND _hWnd, UINT _width, UINT _height)
		{
			m_Width = _width;
			m_Height = _height;

			HRESULT	hr = E_FAIL;
			DXGI_SWAP_CHAIN_DESC sd;
			ZeroMemory(&sd, sizeof(sd));						// ゼロクリア
			sd.BufferDesc.Width = _width;							// バックバッファの幅
			sd.BufferDesc.Height = _height;						// バックバッファの高さ
			sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット(R,G,B,A)
			sd.SampleDesc.Count = 1;							// マルチサンプルの数
			sd.BufferDesc.RefreshRate.Numerator = 1000;
			sd.BufferDesc.RefreshRate.Denominator = 1;
			sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用方法
			sd.BufferCount = 1;									// バックバッファの数
			sd.OutputWindow = _hWnd;							// 関連付けるウインドウ
			sd.Windowed = TRUE;
			sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			// ドライバの種類
			D3D_DRIVER_TYPE driverTypes[] =
			{
				D3D_DRIVER_TYPE_HARDWARE,	// GPUで描画
				D3D_DRIVER_TYPE_WARP,		// 高精度(低速
				D3D_DRIVER_TYPE_REFERENCE,	// CPUで描画
			};
			UINT numDriverTypes = ARRAYSIZE(driverTypes);

			UINT createDeviceFlags = 0;
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

			// 機能レベル
			D3D_FEATURE_LEVEL featureLevels[] =
			{
				D3D_FEATURE_LEVEL_11_1,		// DirectX11.1対応GPUレベル
				D3D_FEATURE_LEVEL_11_0,		// DirectX11対応GPUレベル
				D3D_FEATURE_LEVEL_10_1,		// DirectX10.1対応GPUレベル
				D3D_FEATURE_LEVEL_10_0,		// DirectX10対応GPUレベル
				D3D_FEATURE_LEVEL_9_3,		// DirectX9.3対応GPUレベル
				D3D_FEATURE_LEVEL_9_2,		// DirectX9.2対応GPUレベル
				D3D_FEATURE_LEVEL_9_1		// Direct9.1対応GPUレベル
			};
			UINT numFeatureLevels = ARRAYSIZE(featureLevels);

			D3D_DRIVER_TYPE driverType;
			D3D_FEATURE_LEVEL featureLevel;

			for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
			{
				driverType = driverTypes[driverTypeIndex];
				hr = D3D11CreateDeviceAndSwapChain(
					NULL,					// ディスプレイデバイスのアダプタ（NULLの場合最初に見つかったアダプタ）
					driverType,				// デバイスドライバのタイプ
					NULL,					// ソフトウェアラスタライザを使用する場合に指定する
					createDeviceFlags,		// デバイスフラグ
					featureLevels,			// 機能レベル
					numFeatureLevels,		// 機能レベル数
					D3D11_SDK_VERSION,		// 
					&sd,					// スワップチェインの設定
					&m_pSwapChain,			// IDXGIDwapChainインタフェース	
					&m_pDevice,				// ID3D11Deviceインタフェース
					&featureLevel,		// サポートされている機能レベル
					&m_pContext);		// デバイスコンテキスト
				if (SUCCEEDED(hr)) {
					break;
				}
			}
			if (FAILED(hr)) {
				return false;
			}

			// フレームバッファ用のレンダーターゲットを作成する
			m_pRenderTarget = std::make_shared<lowlevel::RenderTarget>();
			m_pRenderTarget->CreateFromScreen();

			// フレームバッファ用のデプスステンシルを作成する
			m_pDepthStencil = std::make_shared<lowlevel::DepthStencil>();
			m_pDepthStencil->Create(m_Width, m_Height, false);

			// レンダーコンテキストの初期化をする
			m_renderContext = std::make_shared<lowlevel::RenderContext>();
			m_renderContext->Init();

			return true;
		}

		/**
		* @brief グラフィックスシステムの終了処理をします。
		*/
		void Graphics::Shutdown()
		{
			// レンダーコンテキストの終了処理をする
			m_renderContext->Uninit();

			m_pContext->ClearState();
			m_pSwapChain->SetFullscreenState(false, NULL);
		}

		/**
		* @brief レンダリングの開始処理をします。
		* @param _red 背景色の赤成分
		* @param _green 背景色の緑成分
		* @param _blue 背景色の青成分
		*/
		void Graphics::BeginRender(float _red, float _green, float _blue)
		{
			m_renderContext->SetRenderTargets(1, &m_pRenderTarget, m_pDepthStencil);

			m_renderContext->ClearRenderTargetView(m_pRenderTarget, { _red, _green, _blue, 1.0f });
			m_renderContext->ClearDepthStencilView(m_pDepthStencil);
		}

		/**
		* @brief レンダリングの終了処理をします。
		*/
		void Graphics::EndRender()
		{
			m_pSwapChain->Present(0, 0);
		}
	}
}