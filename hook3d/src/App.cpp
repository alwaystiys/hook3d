#include "App.h"
#include <sstream>

App::App() : wnd(800, 600, TEXT("Hook 3D"))
{
}

int App::Loop() {
	MSG msg;
	BOOL gResult;
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages()) {
			return *ecode;
		}

		DoFrame();
		//TranslateMessage(&msg);
		//DispatchMessage(&msg);

		//if (wnd.kbd.KeyIsPressed(VK_SPACE)) {
		//	MessageBoxA(nullptr, "just for test kbd", "title", 0);
		//}
		//if (wnd.kbd.KeyIsPressed(VK_MENU)) {
		//	MessageBoxA(nullptr, "just for test kbd", "title", 0);
		//}

		/*static int i = 0;
		while (!wnd.mouse.IsEmpty()) {
			const auto e = wnd.mouse.Read();
			if (e.has_value()) {
				switch (e.value().GetType())
				{
				case Mouse::Event::Type::Move:
				{
					std::ostringstream oss;
					oss << "Mouse Position " << e.value().GetPosX() << ", " << e.value().GetPosY();
					wnd.SetTitle(oss.str());
					break;
				}
				case Mouse::Event::Type::WheelUp:
				{
					i++;
					std::ostringstream oss;
					oss << "Mouse Up " << i;
					wnd.SetTitle(oss.str());
					break;
				}
				case Mouse::Event::Type::WheelDown:
				{
					i--;
					std::ostringstream oss;
					oss << "Mouse Down " << i;
					wnd.SetTitle(oss.str());
					break;
				}
				}
			}
		}*/

	}
	if (gResult == -1) {
		return -1;
	}
	else {
		return msg.wParam;
	}
}

void App::DoFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time test" << std::setprecision(1) << std::fixed << t;
	wnd.SetTitle(oss.str());

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().DrawTestTriangle();
	wnd.Gfx().EndFrame();

}

