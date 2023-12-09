#ifndef ___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERTARGET_H___
#define ___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERTARGET_H___

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

#include "src/common/core/resource/TextureData.h"

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			class RenderTarget : public resource::TextureData
			{
			public:
				RenderTarget();
				~RenderTarget();
				void Clear();
				void Clear(const float* color);
				HRESULT Create(DXGI_FORMAT format, UINT width, UINT height);
				HRESULT CreateFromScreen();
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetView() const;
			protected:
				virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);
			private:
				Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRTV = nullptr;	// レンダーターゲットビュー
			};
		}
	}
}

#endif //!___SRC_CORE_GRAPHICS_LOWLEVEL_RENDERTARGET_H___