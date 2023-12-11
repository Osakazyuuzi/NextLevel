#ifndef ___SRC_CORE_GRAPHICS_LOWLEVEL_DEPTHSTENCIL_H___
#define ___SRC_CORE_GRAPHICS_LOWLEVEL_DEPTHSTENCIL_H___

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

#include "src/common/core/resource/texture/TextureData.h"

namespace NextLevel
{
	namespace graphics
	{
		namespace lowlevel
		{
			class DepthStencil : public resource::TextureData
			{
			public:
				DepthStencil();
				~DepthStencil();
				void Clear();
				HRESULT Create(UINT width, UINT height, bool useStencil);
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetView() const;
			protected:
				virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& desc, const void* pData = nullptr);
			private:
				Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDSV = nullptr;
			};
		}
	}
}

#endif //!___SRC_CORE_GRAPHICS_LOWLEVEL_DEPTHSTENCIL_H___