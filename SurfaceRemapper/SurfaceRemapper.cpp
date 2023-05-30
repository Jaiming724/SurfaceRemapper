#include "pch.h"

#include "WinToastHandler.h"
using namespace WinToastLib;


HHOOK _hook;
DWORD prev = 0;
WinToast::WinToastError error;
WinToastTemplate templ = WinToastTemplate(WinToastTemplate::ImageAndText02);





LRESULT __stdcall keyboardHook(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		KBDLLHOOKSTRUCT kbdStruct = *((KBDLLHOOKSTRUCT*)lParam);
		if (wParam == WM_KEYDOWN)
		{
			if (kbdStruct.vkCode == 131 && prev == 91)
			{


				INPUT ip;
				ip.type = INPUT_KEYBOARD;
				ip.ki.wScan = 0;
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;

				std::cout << "starting" << std::endl;
				Sleep(500); // pause for 1 second
				// // Press the "Ctrl" key
				ip.ki.wVk =VK_SHIFT;
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));

				// Press the "V" key
				ip.ki.wVk = VK_LWIN;
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));
				ip.ki.wVk = 'S';
				ip.ki.dwFlags = 0; // 0 for key press
				SendInput(1, &ip, sizeof(INPUT));

				// Release the "V" key
				ip.ki.wVk = 'S';
				ip.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ip, sizeof(INPUT));

				// Release the "Ctrl" key
				ip.ki.wVk = VK_LWIN;
				ip.ki.dwFlags = KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));

				ip.ki.wVk = VK_SHIFT;
				ip.ki.dwFlags = KEYEVENTF_KEYUP;
				SendInput(1, &ip, sizeof(INPUT));

				// // Press the "Ctrl" key
				// ip.ki.wVk = VK_CONTROL;
				// ip.ki.dwFlags = 0; // 0 for key press
				// SendInput(1, &ip, sizeof(INPUT));
				//
				// // Press the "V" key
				// ip.ki.wVk = 'V';
				// ip.ki.dwFlags = 0; // 0 for key press
				// SendInput(1, &ip, sizeof(INPUT));
				//
				// // Release the "V" key
				// ip.ki.wVk = 'V';
				// ip.ki.dwFlags = KEYEVENTF_KEYUP;
				// SendInput(1, &ip, sizeof(INPUT));
				//
				// // Release the "Ctrl" key
				// ip.ki.wVk = VK_CONTROL;
				// ip.ki.dwFlags = KEYEVENTF_KEYUP;
				// SendInput(1, &ip, sizeof(INPUT));


			}
			prev = kbdStruct.vkCode;

			std::cout << kbdStruct.vkCode << std::endl;

			if (kbdStruct.vkCode == VK_DELETE)
			{
				PostQuitMessage(0);
			}
		}
	}

	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

void messageLoop()
{
	MSG message;
	while (true)
	{
		int res = PeekMessage(&message, NULL, 0, 0, PM_REMOVE);

		if (res)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
			if (message.message == WM_QUIT)
			{
				break;
			}
		}
	}
}

void __cdecl removeKeyboardHook()
{
	UnhookWindowsHookEx(_hook);
}

DWORD WINAPI keyboardHookFunc(LPVOID lpParm)
{
	HINSTANCE hInstance = GetModuleHandle(NULL);


	HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, hInstance, 0);
	if (!hKeyboardHook)
	{
		MessageBox(NULL, L"Failed to install hook!", L"Error", MB_ICONERROR);
	}
	messageLoop();
	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}


int main()
{
	
	// Set the hook
	WinToast::instance()->setAppName(L"WinToastExample");
	const auto aumi = WinToast::configureAUMI(L"MingLLC", L"SurfaceRemapper", L"wintoastexample", L"20161006");
	WinToast::instance()->setAppUserModelId(aumi);
	if (!WinToast::instance()->initialize())
	{
		std::cout << "Error, could not initialize the lib!" << std::endl;
	}

	templ.setTextField(L"title", WinToastTemplate::FirstLine);
	templ.setTextField(L"subtitle", WinToastTemplate::SecondLine);


	const auto toast_id = WinToast::instance()->showToast(templ, new WinToastHandler(), &error);
	if (toast_id < 0)
	{
		std::wcout << L"Error: Could not launch your toast notification!" << std::endl;
	}
	DWORD dwThread;
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)keyboardHookFunc, nullptr, NULL, &dwThread);
	if (hThread) return WaitForSingleObject(hThread, INFINITE);
	else return 1;
}
