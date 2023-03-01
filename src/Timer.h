#ifndef TIMER_H
#define TIMER_H

#include "includes.h"

class Timer
{
public:
	Timer();

	std::chrono::steady_clock::duration lap();

private:
	std::chrono::steady_clock::time_point start;
};

#endif