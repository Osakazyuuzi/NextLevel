#include "DepthStencil.h"

#include "../Graphics.h"

#pragma comment(lib, "d3d11.lib")

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			DepthStencil::DepthStencil()
				: m_pDSV(nullptr)
			{
			}
			DepthStencil::~DepthStencil()
			{
				m_pDSV.Reset();
				m_pSRV.Reset();
			}
			void DepthStencil::Clear()
			{
				graphics::Graphics::GetInstance().GetDeviceContext()->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			}
			HRESULT DepthStencil::Create(UINT width, UINT height, bool useStencil)
			{
				// https://docs.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-depth-stencil#compositing
				D3D11_TEXTURE2D_DESC desc = MakeTexDesc(useStencil ? DXGI_FORMAT_R24G8_TYPELESS : DXGI_FORMAT_R32_TYPELESS, width, height);
				desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
				return CreateResource(desc);
			}
			Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencil::GetView() const
			{
				return m_pDSV;
			}
			HRESULT DepthStencil::CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData)
			{
				// ステンシル使用判定
				bool useStencil = (desc.Format == DXGI_FORMAT_R24G8_TYPELESS);

				// リソース生成
				desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
				HRESULT hr = resource::TextureData::CreateResource(desc, nullptr);
				if (FAILED(hr)) { return hr; }

				// 設定
				D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
				dsvDesc.Format = useStencil ? DXGI_FORMAT_D24_UNORM_S8_UINT : DXGI_FORMAT_D32_FLOAT;
				dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

				// 生成
				return graphics::Graphics::GetInstance().GetDevice()->CreateDepthStencilView(m_pTex.Get(), &dsvDesc, &m_pDSV);
			}
		}
	}
}