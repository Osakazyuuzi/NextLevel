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
		* @brief �e�N�X�`���̃C���X�^���X��\���N���X�B
		*/
		class TextureInstance
		{
		public:
			/**
			* @brief �R���X�g���N�^�B
			* @param _textureId �e�N�X�`��ID�B
			*/
			TextureInstance(int _textureId = -1)
				: m_offset({ 0, 0 })
				, m_scale({ 1, 1 })
				, m_textureId(_textureId)
			{}

			/**
			* @brief �e�N�X�`��ID��ݒ肵�܂��B
			* @param _textureId �ݒ肷��e�N�X�`��ID�B
			*/
			void SetTextureID(int _textureId)
			{
				m_textureId = _textureId;
			}

			/**
			* @brief �e�N�X�`��ID���擾���܂��B
			* @return int �e�N�X�`��ID�B
			*/
			const int GetTextureID() const noexcept
			{
				return m_textureId;
			}

		private:
			//! �e�N�X�`��ID�B
			int m_textureId;

		public:
			/**
			* @brief UV���W�̃I�t�Z�b�g�l��ݒ肵�܂��B
			* @param _offset �ݒ肷��I�t�Z�b�g�l�B
			*/
			void SetOffset(DirectX::XMFLOAT2 _offset)
			{
				m_offset = _offset;
			}

			/**
			* @brief UV���W�̃I�t�Z�b�g�l���擾���܂��B
			* @return DirectX::XMFLOAT2 �I�t�Z�b�g�l�B
			*/
			const DirectX::XMFLOAT2 GetOffset() const noexcept
			{
				return m_offset;
			}

			/**
			* @brief UV���W�̃X�P�[���l��ݒ肵�܂��B
			* @param _scale �ݒ肷��X�P�[���l�B
			*/
			void SetScale(DirectX::XMFLOAT2 _scale)
			{
				m_scale = _scale;
			}

			/**
			* @brief UV���W�̃X�P�[���l���擾���܂��B
			* @return DirectX::XMFLOAT2 �X�P�[���l�B
			*/
			const DirectX::XMFLOAT2 GetScale() const noexcept
			{
				return m_scale;
			}

		private:
			//! UV���W�̃I�t�Z�b�g�l�B
			DirectX::XMFLOAT2 m_offset = { 0.0f, 0.0f };
			//! UV���W�̃X�P�[���l�B
			DirectX::XMFLOAT2 m_scale = { 1.0f, 1.0f };
		};
	}
}
#endif //!___SRC_COMMON_CORE_RESOURCE_TEXTURE_TEXTUREINSTANCE_H___