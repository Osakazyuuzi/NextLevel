#include "Window.h"

bool Window::m_bCursorEnabled = true;

Window::Window(int _nWidth, int _nHeight, const char* _szName)
	:
	m_nWidth(_nWidth),
	m_nHeight(_nHeight)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsg;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = mc_szClassName;

	RegisterClassEx(&wc);

	RECT wr;
	wr.left = 100;
	wr.right = m_nWidth + wr.left;
	wr.top = 100;
	wr.bottom = m_nHeight + wr.top;
	
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

	m_hWnd = CreateWindowEx(
		0,
		mc_szClassName,
		mc_szClassName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		wr.left,
		wr.top,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		this->m_hInstance,
		NULL
	);

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	UnregisterClass(mc_szClassName, m_hInstance);
	DestroyWindow(m_hWnd);
}

bool Window::ProcessMessages() noexcept
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
	{
		if (!GetMessage(&msg, NULL, 0, 0))
		{
			return false;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return true;
}

void Window::SetEnableCursor(bool _bEnable) noexcept
{
	m_bCursorEnabled = _bEnable;

	if (_bEnable)
	{
		while (ShowCursor(TRUE) < 0);
		ClipCursor(nullptr);
	}
	else
	{
		while (ShowCursor(FALSE) >= 0);
		RECT rect;
		GetClientRect(m_hWnd, &rect);
		MapWindowPoints(m_hWnd, nullptr, 
			reinterpret_cast<POINT*>(&rect), 2);
		ClipCursor(&rect);
	}
}

bool Window::GetCursorEnabled() const noexcept
{
	return m_bCursorEnabled;
}

LRESULT Window::HandleMsg(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam) noexcept
{
	switch (_nMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(_hWnd, _nMsg, _wParam, _lParam);
}
