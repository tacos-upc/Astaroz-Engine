#include "MicroTimer.h"

MicroTimer::MicroTimer()
{
	myCurrentTime = 0;
	running = false;
}

MicroTimer::~MicroTimer()
{
}

void MicroTimer::start()
{
	running = true;
	myStartTime = SDL_GetPerformanceCounter();
	myCurrentTime = 0;
}

Uint32 MicroTimer::read()
{
	if (running) setupCurrentTime();
	return myCurrentTime;
}

void MicroTimer::stop()
{
	if (running)
	{
		running = false;
		setupCurrentTime();
	}
}

void MicroTimer::setupCurrentTime()
{
	myCurrentTime = (SDL_GetPerformanceCounter() - myStartTime) / SDL_GetPerformanceFrequency();
}
