#pragma once
#include <windows.h>
#include <optional>
#include <string>

class Window
{
public:
	Window();
	bool GetWindowState() const;
	HWND MakeWindow();

	static const WCHAR szClassName[11];
	std::wstring caption = L"Game";
	USHORT width = 1280;
	USHORT height = 720;
	HWND hWnd = NULL;
private:
	static LRESULT CALLBACK InitMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void DestroyWindow();

	static HINSTANCE m_hInst;
	static bool m_bInit;
};

