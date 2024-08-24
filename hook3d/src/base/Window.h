#pragma once
#include "utils/SdkConfig.h"
#include "utils/HookException.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include <optional>
#include "Graphics.h"
#include <memory>

class Window {


private:

	class WindowClass
	{
	public:
		static LPCTSTR GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr LPCTSTR wndClassName = TEXT("Hook3D Engine Window");
		static WindowClass wndClass;
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, LPCTSTR name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;
	Graphics& Gfx();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;
	Mouse mouse;
private:
	bool cursorEnabled = true;
	int width;
	int height;
	std::vector<BYTE> rawBuffer;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx;
};