#include "MsTimer.h"


MsTimer::MsTimer()
{
	myCurrentTime = 0;
	running = false;
}

MsTimer::~MsTimer() 
{

}

void MsTimer::start()
{
	running = true;
	myStartTime = SDL_GetTicks();
	myCurrentTime = 0;
}

Uint32 MsTimer::read()
{
	if (running) setupCurrentTime();
	return myCurrentTime;
}

void MsTimer::stop()
{
	if (running)
	{
		running = false;
		setupCurrentTime();
	}
}

void MsTimer::setupCurrentTime()
{
	myCurrentTime = SDL_GetTicks() - myStartTime;
}
