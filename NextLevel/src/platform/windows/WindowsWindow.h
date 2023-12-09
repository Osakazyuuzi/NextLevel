#ifndef ___SRC_COMMON_PLATFORM_WINOWSWINDOW_H___
#define ___SRC_COMMON_PLATFORM_WINOWSWINDOW_H___

#include <Windows.h>
#include "../WindowBase.h"

namespace NextLevel
{
	/**
	* @class WindowsWindow
	* @brief Windows用ウィンドウ管理クラス。
	*/
	class WindowsWindow : public IWindow {
	public:
		/**
		* @brief コンストラクタ。
		* @param _hInstance 
		*/
		WindowsWindow(HINSTANCE _hInstance);

		/**
		* @brief デストラクタ。
		*/
		~WindowsWindow();

		/**
		* @brief ウィンドウを作成します。
		* @param _width ウィンドウの横幅。
		* @param _height ウィンドウの縦幅。
		* @param _title ウィンドウタイトル。
		* @return bool 作成に成功したらtrue。
		*/
		bool Create(
			int					_width,
			int					_height,
			const std::string&	_title) override;

		/**
		* @brief イベントを処理します。
		* @return bool 処理結果。
		*/
		bool ProcessEvent() override;

	private:
		/**
		* @brief ウィンドウプロシージャ。
		* @param _hWnd ウィンドウハンドル。
		* @param _nMsg メッセージ。
		* @param _wParam メッセージに付随する情報。
		* @param _lParam メッセージに付随する情報。
		*/
		static LRESULT CALLBACK HandleMsg(
			HWND				_hWnd, 
			UINT				_nMsg, 
			WPARAM				_wParam, 
			LPARAM				_lParam) noexcept;

	public:
		/**
		* @brief ウィンドウハンドルを取得します。
		* @return HWND ウィンドウハンドル。
		*/
		HWND GetWindowHandle() const noexcept
		{
			return m_hWnd;
		}
	
	private:
		//! ウィンドウインスタンス
		HINSTANCE m_hInstance;
		//! ウィンドウハンドル
		HWND m_hWnd;
	};
}

#endif //!___SRC_CORE_WINDOW_H___