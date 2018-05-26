#include "Settings.h"
#include "resource.h"
#include "OverlappedWindow.h"

CSettings::CSettings() {
	_hwnd = 0;
	_window = 0;
}

bool CSettings::Create(HWND hwnd) {
	_window = hwnd;
	//_control = reinterpret_cast<CNotes*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))->GetControlHwnd();
	CreateDialogParam(
		GetModuleHandle(0),                                              // HINSTANCE    hInstance
		MAKEINTRESOURCE(IDD_DIALOG1),                                    // LPCTSTR      lpTemplateName
		hwnd,                                                            // HWND         hWndParent
		DialogProc,                                                      // DLGPROC      lpDialogFunc
		reinterpret_cast<LPARAM>(this)                                   // LPARAM       dwInitParam
	);
	return (_hwnd != 0);
}

void CSettings::Show() {
	ShowWindow(_hwnd, SW_SHOW);
}

HWND CSettings::GetHwnd() {
	return _hwnd;
}

INT_PTR CSettings::OnCommand(WPARAM wParam) {
	switch (LOWORD(wParam)) {
	case IDOK:
	{
		EndDialog(_hwnd, wParam);
		return 1;
	}
	case IDC_BUTTON_BACKGROUND:
	{
		DWORD color;
		ChangeColor(color);
		COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
		window->SetBackColor(color);
		return 0;
	}
	case IDC_BUTTON_FIRST_PLAYER:
	{
		DWORD color;
		ChangeColor(color);
		COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
		window->SetPlayer1Color(color);
		return 0;
	}
	case IDC_BUTTON_SECOND_PLAYER:
	{
		DWORD color;
		ChangeColor(color);
		COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
		window->SetPlayer2Color(color);
		return 0;
	}
	case IDC_BUTTON_UNDRAWN_POINTS_COLOR:
	{
		DWORD color;
		ChangeColor(color);
		COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
		window->SetUndrawnColor(color);
		return 0;
	}
	default:
	{
		return 1;
	}
	}
	return 0;
}

void CSettings::ChangeColor(DWORD &color) {
  CHOOSECOLOR tmp = {
    sizeof(CHOOSECOLOR),                                             // DWORD        lStructSize
    0,                                                               // HWND         hwndOwner
    0,                                                               // HWND         hInstance
    color,                                                           // COLORREF     rgbResult
    _palette,                                                        // COLORREF     *lpCustColors
    CC_ANYCOLOR | CC_FULLOPEN | CC_RGBINIT,                          // DWORD        Flags
    0,                                                               // LPARAM       lCustData
    0,                                                               // LPCCHOOKPROC lpfnHook
    0                                                                // LPCTSTR      lpTemplateName
  };
  ChooseColor(&tmp);
  color = tmp.rgbResult;
}

void CSettings::OnInit(HWND hwnd) {
	_hwnd = hwnd;
}

INT_PTR CSettings::DialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG:
	{
		CSettings *settings = reinterpret_cast<CSettings*>(lParam);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<long>(settings));
		settings->OnInit(hwnd);
		return 1;
	}
	case WM_COMMAND:
	{
		CSettings *settings = reinterpret_cast<CSettings*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return settings->OnCommand(wParam);
	}
	default:
	{
		return 0;
	}
	}
}