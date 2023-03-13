#ifndef TIMER_H
#define TIMER_H

#include "includes.h"

class Timer
{
public:
	Timer();

	float lap();
	float getTime() const;

private:
	std::chrono::steady_clock::time_point start;
};

#endif
