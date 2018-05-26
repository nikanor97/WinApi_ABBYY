#pragma once
#include <Windows.h>
#include <Commctrl.h>
#include "Preferences.h"
#include "EllipseWindow.h"

class CSettings {
public:
	CSettings();
	bool Create(HWND hwnd);
	void Show();
	HWND GetHwnd();
protected:
	INT_PTR OnCommand(WPARAM wParam);
	void OnInit(HWND hwnd);
private:
	COLORREF _palette[16];
	HWND _hwnd;
	HWND _window;
	void ChangeColor(DWORD &color);
	static INT_PTR CALLBACK DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};