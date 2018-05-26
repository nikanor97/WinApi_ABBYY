#pragma once

#include <Windows.h>
#include <set>
#include "utils.h"
#include "Preferences.h"


class CEllipseWindow {
public:
  CEllipseWindow();
	static bool RegisterClass();
  bool Create(const HWND hwnd, int i, int j);
  void Show(int cmdShow);
  HWND GetHwnd();
protected:
	void OnDestroy();
  void OnCreate();
  void OnLButtonDown();
  void OnNCCreate(const HWND hwnd);
  void OnTimer();
  void OnPaint();
private:
	HWND _hwnd;
	HWND _window;
  UINT_PTR id;
  bool initialized;
  double left;
  double right;
  double top;
  double bottom;
	int i;
	int j;
  static LRESULT __stdcall WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

};