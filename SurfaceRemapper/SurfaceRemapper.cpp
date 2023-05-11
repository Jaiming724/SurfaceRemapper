#include <Windows.h>
#include "iostream"
#include "WinToastHandler.h"
#include "wintoastlib.h"
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
				WinToast::instance()->showToast(templ, new WinToastHandler(), &error);
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
