#include "Timer.h"
using namespace std;
using namespace chrono;

Timer::Timer(): start(steady_clock::now()) {}

float Timer::lap()
{
	const auto old = start;
	start = steady_clock::now();
	return duration_cast<milliseconds>(start - old).count() / 1'000.f;
}

float Timer::getTime() const
{
	return duration_cast<milliseconds>(steady_clock::now() - start).count() / 1'000.f;
}

