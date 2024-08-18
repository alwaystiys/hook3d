#include "HookTimer.h"

using namespace std::chrono;

HookTimer::HookTimer() noexcept
{
	last = steady_clock::now();
}

float HookTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float HookTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}
