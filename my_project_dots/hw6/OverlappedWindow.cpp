#include "OverlappedWindow.h"
#include "resource.h"
#include <iostream>

COverlappedWindow::COverlappedWindow(){
	_settings = CSettings();
	_hwnd = 0;
	_back_br = RGB(255, 150, 70);
	_player1_br = RGB(255, 0, 0);
	_player2_br = RGB(0, 255, 0);
	_undrawn_br = RGB(160, 160, 160);
	player = true;
	restart = false;
	size = 20;
	_state = false;
}

bool COverlappedWindow::RegisterClass() {
  WNDCLASSEX wc = {
    sizeof(WNDCLASSEX),                // UINT      cbSize
    0,                                 // UINT      style
    WindowProc,                        // WNDPROC   lpfnWndProc
    0,                                 // int       cbClsExtra
    0,                                 // int       cbWndExtra
    GetModuleHandle(0),                // HINSTANCE hInstance
    0,                                 // HICON     hIcon
    0,                                 // HCURSOR   hCursor
    0,                                 // HBRUSH    hbrBackground
		MAKEINTRESOURCE(IDR_MENU1),                                 // LPCTSTR   lpszMenuName
    L"Overlapped Window Class",        // LPCTSTR   lpszClassName  
    0                                  // HICON     hIconSm
  };

  return (RegisterClassEx(&wc) != 0);
}

bool COverlappedWindow::Create() {
  CreateWindowEx(
    0,                                           // DWORD     dwExStyle
    L"Overlapped Window Class",                  // LPCTSTR   lpClassName
    L"Points Game",																// LPCTSTR   lpWindowName
    WS_OVERLAPPEDWINDOW,                         // DWORD     dwStyle
    CW_USEDEFAULT,                               // int       x
    CW_USEDEFAULT,                               // int       y
    CW_USEDEFAULT,                               // int       nWidth
    CW_USEDEFAULT,                               // int       nHeight
    0,                                           // HWND      hWndParent
    0,                                           // HMENU     hMenu
    GetModuleHandle(0),                          // HINSTANCE hInstance
    this                                         // LPVOID    lpParam
  );

  return (_hwnd != 0);
}

void COverlappedWindow::Show(int cmdShow) {
	ShowWindow(_hwnd, cmdShow);
  for (auto i = 0; i < FIELD_HEIGHT; ++i) {
    for (auto j = 0; j < FIELD_WIDTH; ++j) {
      _windows[i][j].Show(cmdShow);
    }
  }
}

void COverlappedWindow::OnDestroy() {
  PostQuitMessage(0);
}

void COverlappedWindow::OnCreate() {
  CEllipseWindow::RegisterClass();
  for (auto i = 0; i < FIELD_HEIGHT; ++i) {
    for (auto j = 0; j < FIELD_WIDTH; ++j) {
      _windows[i][j].Create(_hwnd, i, j);
    }
  }
}

void COverlappedWindow::OnNCCreate(const HWND hwnd) {
  _hwnd = hwnd;
}

void COverlappedWindow::OnSize() {
  RECT clientRect;
  GetClientRect(_hwnd, &clientRect);

  int width = (clientRect.right - clientRect.left) / FIELD_WIDTH;
  int height = (clientRect.bottom - clientRect.top) / FIELD_HEIGHT;

  for (auto i = 0; i < FIELD_HEIGHT; ++i) {
    for (auto j = 0; j < FIELD_WIDTH; ++j) {
      int x = clientRect.left + width * j;
      int y = clientRect.top + height * i;
      SetWindowPos(_windows[i][j].GetHwnd(), HWND_TOP, x, y, width, height, 0);
    }
  }
}

void COverlappedWindow::OnCommand(WPARAM wParam) {
	switch (HIWORD(wParam)) {
	case 0:
	{
		ViewSettings(wParam);
		break;
	}
	default:
	{
		break;
	}
	}
}

bool COverlappedWindow::OnClose() {
	if (_state) {
		int code = MessageBox(
			_hwnd,                                               // HWND                  hWnd
			L"Do you want to leave the game, dude?",             // LPCTSTR               lpText
			L"Exit",                                            // LPCTSTR               lpCaption
			MB_YESNO			                                       // UINT                  uType
		);
		switch (code) {
		case IDYES:
		{
			return true;
		}
		case IDNO:
		{
			return false;
		}
		default:
		{
			return true;
		}
		}
	}
}

void COverlappedWindow::ViewSettings(WPARAM wParam) {
	switch (LOWORD(wParam)) {
	case ID_VIEW_SETTINGS:
	{
		_settings.Create(_hwnd);
		_settings.Show();
		break;
	}
	case ID_GAME_ABORT:
	{
		PostQuitMessage(0);
		break;
	}
	case ID_GAME_RESTART:
	{
		restart = true;
		used_pnt_1.clear();
		used_pnt_2.clear();
		break;
	}
	default:
	{
		break;
	}
	}
}

LRESULT __stdcall COverlappedWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_DESTROY:
  {
    COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnDestroy();
    return 0;
  }
  case WM_CREATE:
  {
    COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnCreate();
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  case WM_NCCREATE:
  {
    COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
    SetLastError(0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<long>(window));
    if (GetLastError() != 0) {
      return 42;
    }
    window->OnNCCreate(hwnd);
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  case WM_SIZE:
  {
    COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnSize();
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
	case WM_COMMAND:
	{
		COverlappedWindow *window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		window->OnCommand(wParam);
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
	case WM_CLOSE:
	{
		COverlappedWindow *window = reinterpret_cast<COverlappedWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		if (window->OnClose()) {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		else {
			return 0;
		}
	}
	}
}