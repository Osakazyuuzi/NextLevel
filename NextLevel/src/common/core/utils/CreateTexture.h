#ifndef ___SRC_COMMON_CORE_UTILS_CREATETEXTURE_H___
#define ___SRC_COMMON_CORE_UTILS_CREATETEXTURE_H___

#include <d3d11.h>
#include "third_party/DirectXTex/TextureLoad.h"

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

namespace NextLevel
{
	/**
	* @brief メモリからテクスチャを作成します。
	*/
	HRESULT CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
		_In_bytecount_(wicDataSize) const uint8_t* wicData,
		_In_ size_t wicDataSize,
		_Out_opt_ ID3D11ShaderResourceView** textureView,
		_Out_opt_ DirectX::TexMetadata* pTexInfo = nullptr,
		_In_ bool bMipMap = false
	);
}

#endif //!___SRC_COMMON_CORE_UTILS_CREATETEXTURE_H___