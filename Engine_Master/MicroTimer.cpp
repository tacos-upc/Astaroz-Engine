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
	myStartTime = SDL_GetTicks();
	myCurrentTime = 0;
}

Uint32 MicroTimer::read()
{
	if (running)
	{
		myCurrentTime = SDL_GetTicks() - myStartTime;
	}
	return myCurrentTime;
}

void MicroTimer::stop()
{
	if (running)
	{
		running = false;
		myCurrentTime = SDL_GetTicks() - myStartTime;
	}
}