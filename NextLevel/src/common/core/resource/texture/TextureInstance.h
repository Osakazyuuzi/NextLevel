#ifndef ___SRC_COMMON_CORE_RESOURCE_TEXTURETEXTUREINSTANCE_H___
#define ___SRC_COMMON_CORE_RESOURCE_TEXTURETEXTUREINSTANCE_H___

#include <memory>
#include <iostream>
#include <DirectXMath.h>
#include "TextureData.h"

namespace NextLevel
{
	namespace resource
	{
		/**
		* @class TextureInstance
		* @brief テクスチャのインスタンスを表すクラス。
		*/
		class TextureInstance
		{
		public:
			/**
			* @brief コンストラクタ。
			* @param _textureId テクスチャID。
			*/
			TextureInstance(int _textureId = -1)
				: m_offset({ 0, 0 })
				, m_scale({ 1, 1 })
				, m_textureId(_textureId)
			{}

			/**
			* @brief テクスチャIDを設定します。
			* @param _textureId 設定するテクスチャID。
			*/
			void SetTextureID(int _textureId)
			{
				m_textureId = _textureId;
			}

			/**
			* @brief テクスチャIDを取得します。
			* @return int テクスチャID。
			*/
			const int GetTextureID() const noexcept
			{
				return m_textureId;
			}

		private:
			//! テクスチャID。
			int m_textureId;

		public:
			/**
			* @brief UV座標のオフセット値を設定します。
			* @param _offset 設定するオフセット値。
			*/
			void SetOffset(DirectX::XMFLOAT2 _offset)
			{
				m_offset = _offset;
			}

			/**
			* @brief UV座標のオフセット値を取得します。
			* @return DirectX::XMFLOAT2 オフセット値。
			*/
			const DirectX::XMFLOAT2 GetOffset() const noexcept
			{
				return m_offset;
			}

			/**
			* @brief UV座標のスケール値を設定します。
			* @param _scale 設定するスケール値。
			*/
			void SetScale(DirectX::XMFLOAT2 _scale)
			{
				m_scale = _scale;
			}

			/**
			* @brief UV座標のスケール値を取得します。
			* @return DirectX::XMFLOAT2 スケール値。
			*/
			const DirectX::XMFLOAT2 GetScale() const noexcept
			{
				return m_scale;
			}

		private:
			//! UV座標のオフセット値。
			DirectX::XMFLOAT2 m_offset = { 0.0f, 0.0f };
			//! UV座標のスケール値。
			DirectX::XMFLOAT2 m_scale = { 1.0f, 1.0f };
		};
	}
}
#endif //!___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREINSTANCE_H___