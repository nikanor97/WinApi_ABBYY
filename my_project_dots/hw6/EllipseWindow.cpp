#include "EllipseWindow.h"
#include <set>
#include <vector>
#include <iostream>
#include <utility>
#include "OverlappedWindow.h"
#include "Settings.h"

CEllipseWindow::CEllipseWindow() {
	_window = 0;
  id = 0;
	initialized = 0;
	i = -1;
	j = -1;
}

bool CEllipseWindow::RegisterClass() {
  WNDCLASSEX wc = {
    sizeof(WNDCLASSEX),                          // UINT      cbSize
    0,                                           // UINT      style
    WindowProc,                                  // WNDPROC   lpfnWndProc
    0,                                           // int       cbClsExtra
    0,                                           // int       cbWndExtra
    GetModuleHandle(0),                          // HINSTANCE hInstance
    0,                                           // HICON     hIcon
    0,                                           // HCURSOR   hCursor
    0,                   // HBRUSH    hbrBackground
    0,                                           // LPCTSTR   lpszMenuName
    L"Ellipse Window Class",                     // LPCTSTR   lpszClassName  
    0                                            // HICON     hIconSm
  };

  return (RegisterClassEx(&wc) != 0);
}

bool CEllipseWindow::Create(const HWND hwnd, int i, int j) {
	_window = hwnd;
  CreateWindowEx(
    0,                                           // DWORD     dwExStyle
    L"Ellipse Window Class",                     // LPCTSTR   lpClassName
    L"An example of child window",               // LPCTSTR   lpWindowName
    WS_CHILD | WS_BORDER,                        // DWORD     dwStyle
    0,                                           // int       x
    0,                                           // int       y
    0,                                           // int       nWidth
    0,                                           // int       nHeight
    hwnd,                                        // HWND      hWndParent
    0,                                           // HMENU     hMenu
    GetModuleHandle(0),                          // HINSTANCE hInstance
    this                                         // LPVOID    lpParam
  );

	this->i = i;
	this->j = j;

  return (_hwnd != 0);
}

void CEllipseWindow::Show(int cmdShow) {
  ShowWindow(_hwnd, cmdShow);
}

HWND CEllipseWindow::GetHwnd() {
  return _hwnd;
}

void CEllipseWindow::OnDestroy() {
  KillTimer(_hwnd, id);
  PostQuitMessage(0);
}

void CEllipseWindow::OnCreate() {
  id = SetTimer(_hwnd, 0, 50, 0);
}

void CEllipseWindow::OnLButtonDown() {
  SetFocus(_hwnd);
	COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
	window->SetState(true);
}

void CEllipseWindow::OnNCCreate(const HWND hwnd) {
  _hwnd = hwnd;
}

void CEllipseWindow::OnTimer() {
  RECT rect;
  GetClientRect(_hwnd, &rect);
  InvalidateRect(_hwnd, &rect, FALSE);
}

bool checkPoint(std::set<std::pair<int, int> > &used_pnt, int i, int j) {
	if (i + 1 < FIELD_HEIGHT && j + 1 < FIELD_WIDTH && i - 1 > 0 && j - 1 > 0 &&
		used_pnt.find(std::pair<int, int>(i + 1, j)) != used_pnt.end() &&
		used_pnt.find(std::pair<int, int>(i - 1, j)) != used_pnt.end() &&
		used_pnt.find(std::pair<int, int>(i, j + 1)) != used_pnt.end() &&
		used_pnt.find(std::pair<int, int>(i, j - 1)) != used_pnt.end()) {
		return false;
	}
	else 
		return true;
}

void CEllipseWindow::OnPaint() {
  PAINTSTRUCT ps;
  RECT clientRect;
  
	COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
	HBRUSH back_br = CreateSolidBrush(window->GetBackColor());
	HBRUSH player1_br = CreateSolidBrush(window->GetPlayer1Color());
	HBRUSH player2_br = CreateSolidBrush(window->GetPlayer2Color());
	HBRUSH undrawn_br = CreateSolidBrush(window->GetUndrawnColor());


  HDC hdc = BeginPaint(_hwnd, &ps);

  GetClientRect(_hwnd, &clientRect);

  HDC displayBufferDC = CreateCompatibleDC(hdc);
  HBITMAP displayBuffer = CreateCompatibleBitmap(hdc, clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
  HGDIOBJ oldDisplayBuffer = SelectObject(displayBufferDC, displayBuffer); //where, what

  FillRect(displayBufferDC, &clientRect, back_br);

  

  if (!initialized) {
		COverlappedWindow *window = (COverlappedWindow *)GetWindowLongPtr(_window, GWLP_USERDATA);
		float radius = (float)(window->size) / 100;
    left = clientRect.right * (0.5 - radius);
    top = clientRect.bottom * (0.5 - radius);
    right = clientRect.right * (0.5 + radius);
    bottom = clientRect.bottom * (0.5 + radius);
    initialized = 1;
  }

	
  if (_hwnd == GetFocus() && window->used_pnt_1.find(std::pair<int, int>(i, j)) == window->used_pnt_1.end() &&
		window->used_pnt_2.find(std::pair<int, int>(i, j)) == window->used_pnt_2.end()) {
		window->player = !window->player;
		if (window->player) {
			window->used_pnt_1.insert(std::pair<int, int>(i, j)); // ADD
			SelectObject(displayBufferDC, player1_br);
			if (checkPoint(window->used_pnt_2, i, j) == false) {
				window->used_pnt_2.insert(std::pair<int, int>(i, j)); // ADD
				window->used_pnt_1.erase(window->used_pnt_1.find(std::pair<int, int>(i, j)));
				SelectObject(displayBufferDC, player2_br);
			}
			else if (checkPoint(window->used_pnt_1, i + 1, j) == false) {
				window->used_pnt_1.insert(std::pair<int, int>(i + 1, j));
				window->used_pnt_2.erase(window->used_pnt_2.find(std::pair<int, int>(i + 1, j)));
				SelectObject(displayBufferDC, player1_br);
			}
			else if (checkPoint(window->used_pnt_1, i - 1, j) == false) {
				window->used_pnt_1.insert(std::pair<int, int>(i - 1, j));
				window->used_pnt_2.erase(window->used_pnt_2.find(std::pair<int, int>(i - 1, j)));
				SelectObject(displayBufferDC, player1_br);
			}
			else if (checkPoint(window->used_pnt_1, i, j + 1) == false) {
				window->used_pnt_1.insert(std::pair<int, int>(i, j + 1));
				window->used_pnt_2.erase(window->used_pnt_2.find(std::pair<int, int>(i, j + 1)));
				SelectObject(displayBufferDC, player1_br);
			}
			else if (checkPoint(window->used_pnt_1, i, j - 1) == false) {
				window->used_pnt_1.insert(std::pair<int, int>(i, j - 1));
				window->used_pnt_2.erase(window->used_pnt_2.find(std::pair<int, int>(i, j - 1)));
				SelectObject(displayBufferDC, player1_br);
			}
			else {
				//used_pnt_1.insert(std::pair<int, int>(i, j)); // ADD
				//SelectObject(displayBufferDC, figBrush_1);
			}
		}
		else {
			window->used_pnt_2.insert(std::pair<int, int>(i, j)); // ADD
			SelectObject(displayBufferDC, player2_br);
			if (checkPoint(window->used_pnt_1, i, j) == false) {
				window->used_pnt_1.insert(std::pair<int, int>(i, j)); // ADD
				window->used_pnt_2.erase(window->used_pnt_2.find(std::pair<int, int>(i, j)));
				SelectObject(displayBufferDC, player1_br);
			}
			else if (checkPoint(window->used_pnt_2, i + 1, j) == false) {
				window->used_pnt_2.insert(std::pair<int, int>(i + 1, j));
				window->used_pnt_1.erase(window->used_pnt_1.find(std::pair<int, int>(i + 1, j)));
				SelectObject(displayBufferDC, player2_br);
			}
			else if (checkPoint(window->used_pnt_2, i - 1, j) == false) {
				window->used_pnt_2.insert(std::pair<int, int>(i - 1, j));
				window->used_pnt_1.erase(window->used_pnt_1.find(std::pair<int, int>(i - 1, j)));
				SelectObject(displayBufferDC, player2_br);
			}
			else if (checkPoint(window->used_pnt_2, i, j + 1) == false) {
				window->used_pnt_2.insert(std::pair<int, int>(i, j + 1));
				window->used_pnt_1.erase(window->used_pnt_1.find(std::pair<int, int>(i, j + 1)));
				SelectObject(displayBufferDC, player2_br);
			}
			else if (checkPoint(window->used_pnt_2, i, j - 1) == false) {
				window->used_pnt_2.insert(std::pair<int, int>(i, j - 1));
				window->used_pnt_1.erase(window->used_pnt_1.find(std::pair<int, int>(i, j - 1)));
				SelectObject(displayBufferDC, player2_br);
			}
			else {
				//used_pnt_2.insert(std::pair<int, int>(i, j)); // ADD
				//SelectObject(displayBufferDC, figBrush_2);
			}
		}
	}
  else {
		if (window->used_pnt_1.find(std::pair<int, int>(i, j)) != window->used_pnt_1.end()) {
			SelectObject(displayBufferDC, player1_br);
		}
		else if (window->used_pnt_2.find(std::pair<int, int>(i, j)) != window->used_pnt_2.end())
			SelectObject(displayBufferDC, player2_br);
		else {
			SelectObject(displayBufferDC, undrawn_br);
		}
  }
	if (window->restart) {
		SelectObject(displayBufferDC, undrawn_br);
		window->restart = false;
	}
	Ellipse(displayBufferDC, left, top, right, bottom);

  BitBlt(hdc, clientRect.left, clientRect.top, clientRect.right - clientRect.left,
    clientRect.bottom - clientRect.top, displayBufferDC, 0, 0, SRCCOPY);

  SelectObject(displayBufferDC, oldDisplayBuffer);
  DeleteObject(displayBuffer);
  DeleteDC(displayBufferDC);

  DeleteObject(back_br);
	DeleteObject(player1_br);
	DeleteObject(player2_br);
	DeleteObject(undrawn_br);

  EndPaint(_hwnd, &ps);
}

LRESULT __stdcall CEllipseWindow::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  switch (msg) {
  case WM_DESTROY:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnDestroy();
    return 0;
  }
  case WM_CREATE:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnCreate();
    return DefWindowProc(hwnd, msg, wParam, lParam);;
  }
  case WM_LBUTTONDOWN:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnLButtonDown();
    return 0;
  }
  case WM_NCCREATE:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>((reinterpret_cast<CREATESTRUCT*>(lParam))->lpCreateParams);
    SetLastError(0);
    SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<long>(window));
    if (GetLastError() != 0) {
      return 42;
    }
    window->OnNCCreate(hwnd);
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  case WM_TIMER:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnTimer();
    return 0;
  }
  case WM_PAINT:
  {
    CEllipseWindow* window = reinterpret_cast<CEllipseWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    window->OnPaint();
    return 0;
  }
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
}