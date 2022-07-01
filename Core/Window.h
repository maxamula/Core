#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "Core.h"
#include <windows.h>
#include <optional>

class Core;

class Window
{
	friend class Core;
public:
	Window(Core* core);
	HWND WinId() const;
	void SetCaption(WCHAR* szCaption);
	void SetWindowSize(USHORT width, USHORT height);
	USHORT GetWidth() const;
	USHORT GetHeight() const;
	bool GetWindowState() const;
private:
	static LRESULT CALLBACK InitMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMsgProxy(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	std::optional<int> ProcessMessages();
	HWND MakeWindow();
	void DestroyWindow();

	static HINSTANCE m_hInst;
	static const WCHAR szClassName[11];
	static bool m_bInit;
	Core* m_core;
	HWND m_hWnd = NULL;
	USHORT m_width = 1280;
	USHORT m_height = 720;
	WCHAR m_caption[60] =  L"Game";
	Keyboard& m_keyboard;
	Mouse& m_mouse;
};

