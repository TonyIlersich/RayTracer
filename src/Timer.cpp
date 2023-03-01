#include "Timer.h"
using namespace std;

Timer::Timer(): start(chrono::steady_clock::now()) {}

chrono::steady_clock::duration Timer::lap()
{
	const auto old = start;
	start = chrono::steady_clock::now();
	return start - old;
}
