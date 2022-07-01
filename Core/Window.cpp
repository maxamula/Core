#include "Window.h"

HINSTANCE Window::m_hInst = NULL;
const WCHAR Window::szClassName[11] = L"GameWindow";
bool Window::m_bInit = false;

Window::Window(Core* core) : m_keyboard(core->input().keyboard), m_mouse(core->input().mouse), m_core(core)
{
	if (!m_bInit)
	{
		m_hInst = GetModuleHandle(NULL);
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.hInstance = m_hInst;
		wc.lpszClassName = szClassName;
		wc.lpfnWndProc = Window::InitMessageHandler;
		wc.style = 0;
		wc.hIcon = NULL;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		if (!RegisterClass(&wc))
		{
			MessageBox(NULL, L"An error occured", L"Error", MB_OK);
		}
		m_bInit = true;
	}
}

HWND Window::WinId() const
{
	return m_hWnd;
}

void Window::SetCaption(WCHAR* szCaption)
{
	UINT nChars = wcslen(szCaption);
	nChars > 60 ? memcpy(m_caption, szCaption, 60u*sizeof(WCHAR)) : memcpy(m_caption, szCaption, nChars*sizeof(WCHAR));
	if (m_hWnd != NULL)
		SetWindowText(m_hWnd, m_caption);
}

void Window::SetWindowSize(USHORT width, USHORT height)
{
	m_width = width;
	m_height = height;
	if (m_hWnd != NULL)
	{
		RECT oldPos;
		GetWindowRect(m_hWnd, &oldPos);
		if (SetWindowPos(m_hWnd, HWND_TOP, oldPos.left, oldPos.top, width, height + 36, 0) == NULL)
			SetWindowPos(m_hWnd, HWND_TOP, oldPos.left, oldPos.top, width, height + 36, SWP_ASYNCWINDOWPOS);
	}
}

USHORT Window::GetWidth() const
{
	return m_width;
}

USHORT Window::GetHeight() const
{
	return m_height;
}

bool Window::GetWindowState() const
{
	if (m_hWnd == NULL)
		return false;
	else
		return true;
}

LRESULT WINAPI Window::InitMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		CREATESTRUCTW* pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* pWnd = (Window*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWnd);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)Window::HandleMsgProxy);
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		m_keyboard.ClearState();
		break;
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || m_keyboard.Autorepeat())
		{
			m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || m_keyboard.Autorepeat())
		{
			m_keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		m_keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_SYSKEYUP:
		m_keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		m_keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < m_width && pt.y >= 0 && pt.y < m_height)
		{
			m_mouse.OnMouseMove(pt.x, pt.y);
			if (!m_mouse.IsInWindow())
			{
				SetCapture(hWnd);
				m_mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				m_mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				m_mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		m_mouse.OnRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= m_width || pt.y < 0 || pt.y >= m_height)
		{
			ReleaseCapture();
			m_mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		m_mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::MakeWindow()
{
	if (m_hWnd != NULL)
		::DestroyWindow(m_hWnd);
	m_hWnd = CreateWindow(szClassName, m_caption, WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, m_width, m_height, NULL, NULL, m_hInst, this);
	m_core->gfx().CreateInterfaces(m_hWnd);
	return m_hWnd;
}

void Window::DestroyWindow()
{
	::DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}

std::optional<int> Window::ProcessMessages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}