#include <Windows.h>
#include <deque>
#include <iostream>

std::deque<DWORD> clicks;
bool cpsLimiterEnabled = true;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0 && wParam == WM_LBUTTONDOWN) {
  	if (!cpsLimiterEnabled)
  		 return CallNextHookEx(NULL, nCode, wParam, lParam);
    DWORD now = GetTickCount();
    DWORD delay = now - clicks.front();
    while (clicks.size() > 0 && delay > 1000) {
      clicks.pop_front();
    }

    if (clicks.size() >= 16) {
      return 1;
    }
    clicks.push_back(now);
  }

  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0 && wParam == WM_KEYDOWN) {
    KBDLLHOOKSTRUCT *key = (KBDLLHOOKSTRUCT *)lParam;
    if (key->vkCode == VK_F4) {
      cpsLimiterEnabled = !cpsLimiterEnabled;
		if (!cpsLimiterEnabled)
      		std::cout << "desac" << std::endl;
      	else
      		std::cout << "active" << std::endl;
    }
  }

  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
	HHOOK hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
  
	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);


  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  UnhookWindowsHookEx(hook);
  UnhookWindowsHookEx(keyboardHook);

  return 0;
}
