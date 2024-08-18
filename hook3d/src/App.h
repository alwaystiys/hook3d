#pragma once
#include "base/Window.h"

class App
{
public:
	App();
	int Loop();
private:
	void DoFrame();
private:
	Window wnd;
};

