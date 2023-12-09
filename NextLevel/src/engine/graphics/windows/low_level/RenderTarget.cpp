#include "RenderTarget.h"

#include "../Graphics.h"

#pragma comment(lib, "d3d11.lib")

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			RenderTarget::RenderTarget()
				: m_pRTV(nullptr)
			{
			}
			RenderTarget::~RenderTarget()
			{
				m_pRTV.Reset();
				m_pSRV.Reset();
			}
			void RenderTarget::Clear()
			{
				static float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
				Clear(color);
			}
			void RenderTarget::Clear(const float* color)
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearRenderTargetView(m_pRTV.Get(), color);
			}
			HRESULT RenderTarget::Create(DXGI_FORMAT format, UINT width, UINT height)
			{
				D3D11_TEXTURE2D_DESC desc = MakeTexDesc(format, width, height);
				desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
				return CreateResource(desc);
			}
			HRESULT RenderTarget::CreateFromScreen()
			{
				HRESULT hr;

				// バックバッファのポインタを取得
				ID3D11Texture2D* pBackBuffer = NULL;
				hr = graphics::Graphics::GetInstance().GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pTex);
				if (FAILED(hr)) { return hr; }

				// バックバッファへのポインタを指定してレンダーターゲットビューを作成
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
				rtvDesc.Texture2D.MipSlice = 0;
				hr = graphics::Graphics::GetInstance().GetDevice()->CreateRenderTargetView(m_pTex.Get(), &rtvDesc, &m_pRTV);
				if (SUCCEEDED(hr))
				{
					D3D11_TEXTURE2D_DESC desc;
					m_pTex->GetDesc(&desc);
					m_width = desc.Width;
					m_height = desc.Height;
				}
				return hr;
			}
			Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTarget::GetView() const
			{
				return m_pRTV;
			}
			HRESULT RenderTarget::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
			{
				// テクスチャリソース作成
				HRESULT hr = resource::TextureData::CreateResource(desc, nullptr);
				if (FAILED(hr)) { return hr; }

				// 設定
				D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
				rtvDesc.Format = desc.Format;
				rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

				// 生成
				return graphics::Graphics::GetInstance().GetDevice()->CreateRenderTargetView(m_pTex.Get(), &rtvDesc, &m_pRTV);
			}
		}
	}
}