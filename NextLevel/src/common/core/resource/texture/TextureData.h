#ifndef ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREDATA_H___
#define ___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREDATA_H___

#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class TextureData
		* @brief テクスチャデータを管理するクラス。
		*/
		class TextureData
		{
		public:
			/**
			* @brief コンストラクタ。
			*/
			TextureData();

			/**
			* @brief デストラクタ。
			*/
			virtual ~TextureData();

			/**
			 * @brief ファイルからテクスチャを作成します。
			 * @param _fileName テクスチャファイルの名前。
			 * @return HRESULT 操作の結果。
			 */
			HRESULT Create(const char* _fileName);

			/**
			 * @brief メモリからテクスチャを作成します。
			 * @param _format テクスチャのフォーマット。
			 * @param _width テクスチャの幅。
			 * @param _height テクスチャの高さ。
			 * @param _pData テクスチャデータへのポインタ。
			 * @return HRESULT 操作の結果。
			 */
			HRESULT Create(DXGI_FORMAT _format, UINT _width, UINT _height, const void* _pData = nullptr);

			/**
			* @brief テクスチャの横幅を取得します。
			* @return UINT テクスチャの横幅。
			*/
			const UINT GetWidth() const noexcept
			{
				return m_width;
			}

			/**
			* @brief テクスチャの縦幅を取得します。
			* @return UINT テクスチャの縦幅。
			*/
			const UINT GetHeight() const noexcept
			{
				return m_height;
			}

			/**
			 * @brief シェーダーリソースビューのポインタを取得します、
			 * @return Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> シェーダーリソースビューのポインタ。
			 */
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetResource() const
			{
				return m_pSRV;
			}

		protected:
			/**
			 * @brief テクスチャ記述子を作成します。
			 * @param _format テクスチャのフォーマット。
			 * @param _width テクスチャの幅幅。
			 * @param _height テクスチャの縦幅。
			 * @return D3D11_TEXTURE2D_DESC テクスチャ情報。
			 */
			D3D11_TEXTURE2D_DESC MakeTexDesc(DXGI_FORMAT _format, UINT _width, UINT _height);

			/**
			 * @brief リソースを作成します。
			 * @param _desc テクスチャ情報。
			 * @param _pData テクスチャデータへのポインタ。
			 * @return HRESULT 操作の結果。
			 */
			virtual HRESULT CreateResource(D3D11_TEXTURE2D_DESC& _desc, const void* _pData);

		protected:
			//! テクスチャの横幅。
			UINT m_width;
			//! テクスチャの縦幅。
			UINT m_height;
			//! DirectX11 シェーダーリソースビュー。
			Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
			//! DirectX11 テクスチャバッファ。
			Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTex;
		};
	}
}

#endif //!___SRC_COMMON_CORE_RESOURCE TEXTURE_TEXTUREDATA_H___