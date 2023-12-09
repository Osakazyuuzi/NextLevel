#ifndef ___SRC_COMMON_PLATFORM_WINDOWBASE_H___
#define ___SRC_COMMON_PLATFORM_WINDOWBASE_H___

#include <string>
#include <optional>

namespace NextLevel
{
	/**
	* @class IWindow
	* @brief �E�B���h�E�N���X�̊��N���X�B
	*	�S�ẴE�B���h�E�͂��̃N���X���p������B
	*/
	class IWindow {
	public:
		/**
		* @brief �R���X�g���N�^�B
		*/
		IWindow() = default;

		/**
		* @brief �f�X�g���N�^�B
		*/
		~IWindow() = default;

		/**
		* @brief �E�B���h�E���쐬���܂��B
		* @param _width �E�B���h�E�̉����B
		* @param _height �E�B���h�E�̏c���B
		* @param _title �E�B���h�E�̃^�C�g�����B
		*/
		virtual bool Create(
			int					_width,
			int					_height,
			const std::string&	_title) = 0;

		/**
		* @brief �C�x���g�����s���܂��B
		*/
		virtual bool ProcessEvent() = 0;

		/**
		* @brief �E�B���h�E�̉������擾���܂��B
		* @return int �E�B���h�E�̉����B
		*/
		virtual int GetWidth() const noexcept
		{
			return m_nWidth;
		}

		/**
		* @brief �E�B���h�E�̏c�����擾���܂��B
		* @return int �E�B���h�E�̏c���B
		*/
		virtual int GetHeight() const noexcept
		{
			return m_nHeight;
		}

	protected:
		//! �E�B���h�E�̉���
		int m_nWidth;
		//! �E�B���h�E�̏c��
		int m_nHeight;
		//! �E�B���h�E�^�C�g��
		std::string m_szTitle;
	};
}

#endif //!___SRC_COMMON_PLATFORM_WINOW_BASE_H___