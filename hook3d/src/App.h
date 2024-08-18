#pragma once
#include "base/Window.h"
#include "base/HookTimer.h"

class App
{
public:
	App();
	int Loop();
private:
	void DoFrame();
private:
	Window wnd;
	HookTimer timer;
};

