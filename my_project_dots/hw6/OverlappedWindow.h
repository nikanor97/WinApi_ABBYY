#pragma once

#include <Windows.h>
#include <array>
#include "EllipseWindow.h"
#include "Settings.h"
#include "utils.h"

class COverlappedWindow {
public:
	COverlappedWindow();
  static bool RegisterClass();
  bool Create();
  void Show(int cmdShow);

	DWORD GetBackColor() { return _back_br; };
	DWORD GetPlayer1Color() { return _player1_br; };
	DWORD GetPlayer2Color() { return _player2_br; };
	DWORD GetUndrawnColor() { return _undrawn_br; };
	int GetSize() { return size; };
	void SetBackColor(DWORD back_br) { _back_br = back_br; };
	void SetPlayer1Color(DWORD player1_br) { _player1_br = player1_br; };
	void SetPlayer2Color(DWORD player2_br) { _player2_br = player2_br; };
	void SetUndrawnColor(DWORD undrawn_br) { _undrawn_br = undrawn_br; };
	void SetState(bool state) { _state = state; };
	void SetSize(int radius) { size = radius; };
	bool restart;
	int player;
	float size;
	std::set<std::pair<int, int> > used_pnt_1;
	std::set<std::pair<int, int> > used_pnt_2;

protected:
	bool OnClose();
  void OnDestroy();
  void OnCreate();
  void OnNCCreate(const HWND hwnd);
  void OnSize();
	void OnCommand(WPARAM);
private:
	bool _state;
	DWORD _back_br;
	DWORD _player1_br;
	DWORD _player2_br;
	DWORD _undrawn_br;
	HWND _hwnd;
	CSettings _settings;
	std::array<std::array<CEllipseWindow, FIELD_WIDTH>, FIELD_HEIGHT> _windows;
  static LRESULT __stdcall WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void ViewSettings(WPARAM);
};