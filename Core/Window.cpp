#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
// imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

extern Keyboard keyboard;
extern Mouse mouse;
extern Graphics graphics;
extern ImGuiIO imGuiIO;

HINSTANCE Window::m_hInst = NULL;
const WCHAR Window::szClassName[11] = L"GameWindow";
bool Window::m_bInit = false;

Window::Window()
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

bool Window::GetWindowState() const
{
	if (hWnd == NULL)
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

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow();
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		keyboard.ClearState();
		break;
	case WM_KEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.Autorepeat())
		{
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || keyboard.Autorepeat())
		{
			keyboard.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_SYSKEYUP:
		keyboard.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		keyboard.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		SetForegroundWindow(hWnd);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

HWND Window::MakeWindow()
{
	if (hWnd != NULL)
		::DestroyWindow(hWnd);
	RECT rect = {0, 0, width, height}; // will contain agjusted window rect
	AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW, 0, 0);	// resize window considering borders
	hWnd = CreateWindow(szClassName, caption.c_str(), WS_MINIMIZEBOX | WS_SYSMENU | WS_OVERLAPPED, CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, m_hInst, this);
	// create d3d device etc
	graphics.CreateInterfaces(hWnd, {width, height});
	// initialize imgui
	ImGui_ImplDX11_Init(graphics.pDev.Get(), graphics.pContext.Get());
	ImGui_ImplWin32_Init(hWnd);
	// Set imgui coords
	imGuiIO.DisplaySize = { (float)width, (float)height };
	return hWnd;
}

void Window::DestroyWindow()
{
	::DestroyWindow(hWnd);
	hWnd = NULL;
}