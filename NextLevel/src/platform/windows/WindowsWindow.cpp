#include "WindowsWindow.h"

#include "third_party/ImGui/imgui.h"
#include "third_party/ImGui/imgui_impl_dx11.h"
#include "third_party/ImGui/imgui_impl_win32.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

namespace NextLevel
{
	/**
	* @brief �R���X�g���N�^�B
	* @param _hInstance
	*/
	WindowsWindow::WindowsWindow(HINSTANCE _hInstance)
		: m_hInstance(_hInstance)
	{
	}

	/**
	* @brief �f�X�g���N�^�B
	*/
	WindowsWindow::~WindowsWindow()
	{
		UnregisterClass(m_szTitle.c_str(), m_hInstance);
		if (m_hWnd != nullptr)
			DestroyWindow(m_hWnd);
	}

	/**
	* @brief �E�B���h�E���쐬���܂��B
	* @param _width �E�B���h�E�̉����B
	* @param _height �E�B���h�E�̏c���B
	* @param _title �E�B���h�E�^�C�g���B
	* @return bool �쐬�ɐ���������true�B
	*/
	bool WindowsWindow::Create(int _width, int _height, const std::string& _title)
	{
		m_nWidth = _width;
		m_nHeight = _height;
		m_szTitle = _title;

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
		wc.lpszClassName = m_szTitle.c_str();

		RegisterClassEx(&wc);

		RECT wr;
		wr.left = 20;
		wr.right = m_nWidth + wr.left;
		wr.top = 45;
		wr.bottom = m_nHeight + wr.top;

		AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		m_hWnd = CreateWindowEx(
			0,
			m_szTitle.c_str(),
			m_szTitle.c_str(),
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
		return true;
	}

	/**
	* @brief �C�x���g���������܂��B
	* @return bool �������ʁB
	*/
	bool WindowsWindow::ProcessEvent()
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

	/**
	* @brief �E�B���h�E�v���V�[�W���B
	* @param _hWnd �E�B���h�E�n���h���B
	* @param _nMsg ���b�Z�[�W�B
	* @param _wParam ���b�Z�[�W�ɕt��������B
	* @param _lParam ���b�Z�[�W�ɕt��������B
	*/
	LRESULT WindowsWindow::HandleMsg(HWND _hWnd, UINT _nMsg, WPARAM _wParam, LPARAM _lParam) noexcept
	{
		if (ImGui_ImplWin32_WndProcHandler(_hWnd, _nMsg, _wParam, _lParam))
		{
			return true;
		}

		switch (_nMsg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(_hWnd, _nMsg, _wParam, _lParam);
	}

}