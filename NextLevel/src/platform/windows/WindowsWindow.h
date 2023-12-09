#ifndef ___SRC_COMMON_PLATFORM_WINOWSWINDOW_H___
#define ___SRC_COMMON_PLATFORM_WINOWSWINDOW_H___

#include <Windows.h>
#include "../WindowBase.h"

namespace NextLevel
{
	/**
	* @class WindowsWindow
	* @brief Windows�p�E�B���h�E�Ǘ��N���X�B
	*/
	class WindowsWindow : public IWindow {
	public:
		/**
		* @brief �R���X�g���N�^�B
		* @param _hInstance 
		*/
		WindowsWindow(HINSTANCE _hInstance);

		/**
		* @brief �f�X�g���N�^�B
		*/
		~WindowsWindow();

		/**
		* @brief �E�B���h�E���쐬���܂��B
		* @param _width �E�B���h�E�̉����B
		* @param _height �E�B���h�E�̏c���B
		* @param _title �E�B���h�E�^�C�g���B
		* @return bool �쐬�ɐ���������true�B
		*/
		bool Create(
			int					_width,
			int					_height,
			const std::string&	_title) override;

		/**
		* @brief �C�x���g���������܂��B
		* @return bool �������ʁB
		*/
		bool ProcessEvent() override;

	private:
		/**
		* @brief �E�B���h�E�v���V�[�W���B
		* @param _hWnd �E�B���h�E�n���h���B
		* @param _nMsg ���b�Z�[�W�B
		* @param _wParam ���b�Z�[�W�ɕt��������B
		* @param _lParam ���b�Z�[�W�ɕt��������B
		*/
		static LRESULT CALLBACK HandleMsg(
			HWND				_hWnd, 
			UINT				_nMsg, 
			WPARAM				_wParam, 
			LPARAM				_lParam) noexcept;

	public:
		/**
		* @brief �E�B���h�E�n���h�����擾���܂��B
		* @return HWND �E�B���h�E�n���h���B
		*/
		HWND GetWindowHandle() const noexcept
		{
			return m_hWnd;
		}
	
	private:
		//! �E�B���h�E�C���X�^���X
		HINSTANCE m_hInstance;
		//! �E�B���h�E�n���h��
		HWND m_hWnd;
	};
}

#endif //!___SRC_CORE_WINDOW_H___