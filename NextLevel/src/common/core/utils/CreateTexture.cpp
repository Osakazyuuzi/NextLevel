#include "CreateTexture.h"


#define _CRT_SECURE_NO_WARNINGS
#include <memory.h>
#include <stdlib.h>

#pragma comment(lib, "DirectXTex")

using namespace DirectX;
using namespace NextLevel;

// ƒƒ‚ƒŠ‚©‚ç¶¬
HRESULT CreateTextureFromMemory(_In_ ID3D11Device* d3dDevice,
	_In_bytecount_(wicDataSize) const uint8_t* wicData,
	_In_ size_t wicDataSize,
	_Out_opt_ ID3D11ShaderResourceView** textureView,
	_Out_opt_ TexMetadata* pTexInfo,
	_In_ bool bMipMap)
{
	*textureView = nullptr;
	TexMetadata meta;
	ScratchImage image;
	HRESULT hr;
	if (wicDataSize >= 18 && memcmp(&wicData[wicDataSize - 18], "TRUEVISION-XFILE.", 18) == 0) {
		hr = LoadFromTGAMemory(wicData, wicDataSize, &meta, image);
	}
	else if (wicDataSize >= 4 && memcmp(wicData, "DDS ", 4) == 0) {
		hr = LoadFromDDSMemory(wicData, wicDataSize, DDS_FLAGS_FORCE_RGB, &meta, image);
	}
	else {
		hr = LoadFromWICMemory(wicData, wicDataSize, WIC_FLAGS_IGNORE_SRGB, &meta, image);
	}
	if (FAILED(hr)) return hr;
	if (bMipMap) {
		ScratchImage mipmap;
		hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), meta, TEX_FILTER_DEFAULT, 0, mipmap);
		if (FAILED(hr)) return hr;
		meta = mipmap.GetMetadata();
		if (pTexInfo) *pTexInfo = meta;
		return CreateShaderResourceView(d3dDevice, mipmap.GetImages(), mipmap.GetImageCount(), meta, textureView);
	}
	if (pTexInfo) *pTexInfo = meta;
	return CreateShaderResourceView(d3dDevice, image.GetImages(), image.GetImageCount(), meta, textureView);
}