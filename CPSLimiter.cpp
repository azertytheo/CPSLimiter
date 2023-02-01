#include <vector>
#include <random>
#include <algorithm>
#include <deque>
#include <Windows.h>
#include <iostream>

std::deque<DWORD> clicks;
int index = 0;
std::vector<bool> list;
bool cpsLimiterEnabled = true;
HHOOK hook;

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0 && wParam == WM_LBUTTONDOWN) {
  	if (index == 10)
        index = 0;
    DWORD now = GetTickCount();
    while (!clicks.empty() && now - clicks.front() > 1000) {
      clicks.pop_front();
    }

    if (!clicks.empty() && now - clicks.back() < 50) {
      if (!list[index++]) {
        return 1;
	  }
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
		if (!cpsLimiterEnabled) {
			hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
			std::cout << "desac" << std::endl;
		}
      		
      	else {
		  	UnhookWindowsHookEx(hook);
      		std::cout << "active" << std::endl;
      	}
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

std::vector<bool> randomList(int size, int numFalses) {
    std::vector<bool> listee(size, true);
	for (int i = 0; i < numFalses; i++)
        listee[i] = false;
    std::mt19937 rng(std::random_device{}());
    std::shuffle(listee.begin(), listee.end(), rng);
    return listee;
}

int main() {
  int numFalses;
  std::cout << "How much clicks do you want to remove: ";
  std::cin >> numFalses;
	list = randomList(10, numFalses);
  hook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
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
