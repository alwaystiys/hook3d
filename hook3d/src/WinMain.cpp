#include <Windows.h>
#include "utils/WindowsMessageMap.h"
#include <sstream>
#include <tchar.h>
#include <comdef.h>
#include <base/Window.h>
#include "App.h"
//#include "Window.h"

//LPCWSTR StringToLPCWSTR(const std::string& str)
//{
//	int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
//	wchar_t* buffer = new wchar_t[size];
//	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, size);
//	return buffer;
//}
//std::wstring s2ws(const std::string& s)
//{
//	int len;
//	int slength = (int)s.length() + 1;
//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
//	wchar_t* buf = new wchar_t[len];
//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
//	std::wstring r(buf);
//	delete[] buf;
//	return r;
//}

//LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	static WindowsMessageMap mm;
//	OutputDebugStringA(mm(msg, lParam, wParam).c_str());
//
//	switch (msg)
//	{
//	case WM_CLOSE:
//		PostQuitMessage(69);
//		break;
//	case WM_KEYDOWN:
//		if (wParam == 'F') {
//			SetWindowText(hWnd, TEXT("Title Changed"));
//		}
//		break;
//	case WM_CHAR:
//		//static std::string title;
//		//title.push_back((char)wParam);
//		//SetWindowText(hWnd, StringToLPCWSTR(title));
//		break;
//	case WM_LBUTTONDOWN:
//		POINTS pt = MAKEPOINTS(lParam);
//		std::ostringstream oss;
//		oss << "(" << pt.x << "," << pt.y << ")";
//		SetWindowTextA(hWnd, (oss.str().c_str()));
//		break;
//	}
//
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,      // handle to current instance
	_In_opt_ HINSTANCE hPrevInstance,  // handle to previous instance
	_In_ LPSTR lpCmdLine,          // command line
	_In_ int nCmdShow              // show state
)
{

	return App().Loop();
	//const auto pClassName = TEXT("hook3d");
	//const auto pWindowName = TEXT("Hook 3D");

	//// register window class
	//WNDCLASSEX wc = { 0 };
	//wc.cbSize = sizeof(wc);
	//wc.style = CS_OWNDC;
	//wc.lpfnWndProc = WndProc;
	//wc.cbClsExtra = 0;
	//wc.cbWndExtra = 0;
	//wc.hInstance = hInstance;
	//wc.hCursor = nullptr;
	//wc.hIcon = nullptr;
	//wc.hbrBackground = nullptr;
	//wc.lpszMenuName = nullptr;
	//wc.hIconSm = nullptr;
	//wc.lpszClassName = pClassName;
	//RegisterClassEx(&wc);

	// create window instance
	//HWND hWnd = CreateWindowEx(
	//	0,
	//	pClassName,
	//	pWindowName,
	//	WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
	//	200,
	//	200,
	//	720,
	//	480,
	//	nullptr,
	//	nullptr,
	//	hInstance,
	//	nullptr
	//);
	//ShowWindow(hWnd, SW_SHOW);
	//MSG msg;
	//BOOL gResult;
	//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}
	//if (gResult == -1) {
	//	return -1;
	//}
	//else {
	//	return msg.wParam;
	//}
	//Window app(880, 480, TEXT("Hook 3D"));
	//MSG msg;
	//BOOL gResult;
	//while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);

	//	/*	if (app.kbd.KeyIsPressed(VK_SPACE)) {
	//			MessageBoxA(nullptr, "just for test kbd", "title", 0);
	//		}*/
	//	if (app.kbd.KeyIsPressed(VK_MENU)) {
	//		MessageBoxA(nullptr, "just for test kbd", "title", 0);
	//	}

	//	static int i = 0;
	//	while (!app.mouse.IsEmpty()) {
	//		const auto e = app.mouse.Read();
	//		if (e.has_value()) {
	//			switch (e.value().GetType())
	//			{
	//			case Mouse::Event::Type::Move:
	//			{
	//				std::ostringstream oss;
	//				oss << "Mouse Position " << e.value().GetPosX() << ", " << e.value().GetPosY();
	//				app.SetTitle(oss.str());
	//				break;
	//			}
	//			case Mouse::Event::Type::WheelUp:
	//			{
	//				i++;
	//				std::ostringstream oss;
	//				oss << "Mouse Up " << i;
	//				app.SetTitle(oss.str());
	//				break;
	//			}
	//			case Mouse::Event::Type::WheelDown:
	//			{
	//				i--;
	//				std::ostringstream oss;
	//				oss << "Mouse Down " << i;
	//				app.SetTitle(oss.str());
	//				break;
	//			}
	//			}


	//		}
	//	}

	//}
	//if (gResult == -1) {
	//	return -1;
	//}
	//else {
	//	return msg.wParam;
	//}
}