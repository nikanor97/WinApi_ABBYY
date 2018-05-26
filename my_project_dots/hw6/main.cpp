#include "OverlappedWindow.h"

//16:00 friday

int wWinMain(HINSTANCE instance, HINSTANCE, wchar_t *commandLine, int cmdShow) {

	COverlappedWindow w;
  w.RegisterClass();
  w.Create();
  w.Show(cmdShow);
  MSG msg = {};
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}