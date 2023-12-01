#ifndef ___SRC_CORE_WINDOW_H___
#define ___SRC_CORE_WINDOW_H___

#include <Windows.h>

class Window {
public:
	Window(int _nWidth, int _nHeight, const char* _szName);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	bool ProcessMessages() noexcept;
	void SetEnableCursor(bool _bEnable) noexcept;
	bool GetCursorEnabled() const noexcept; 
private:
	static constexpr const char* mc_szClassName = "NextLevel";
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int m_nWidth;
	int m_nHeight;
	static bool m_bCursorEnabled;
private:
	static LRESULT CALLBACK HandleMsg(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam) noexcept;
};

#endif //!___SRC_CORE_WINDOW_H___