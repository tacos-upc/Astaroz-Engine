#include "ModuleTimer.h"
#include <SDL.h>



ModuleTimer::ModuleTimer()
{
}


ModuleTimer::~ModuleTimer()
{
}

bool ModuleTimer::Init()
{
	framesSinceStartUp = 0;
	timeSinceStartUpInSeconds = 0;

	framesSinceGamestart = 0;
	realTimeSinceGameStart = 0;
	gameTimeSinceGameStart = 0;

	realDeltaTime = 0;
	gameDeltaTime = 0;

	timeScale = 1;

	timeState = TIME_STOP;

	return true;
}

void ModuleTimer::play()
{
	timeState = TIME_PLAY;
}

void ModuleTimer::pause()
{
	timeState = TIME_PAUSE;
}

void ModuleTimer::stop()
{
	timeState = TIME_STOP;
}

void ModuleTimer::moveFramesForward(int)
{
}

void ModuleTimer::moveFramesBackward(int)
{
}

void ModuleTimer::Cleanup()
{
}

void ModuleTimer::beginFrame()
{
}

void ModuleTimer::endFrame()
{
	framesSinceStartUp = SDL_GetTicks();
	if (timeState == TIME_PLAY) framesSinceGamestart += 1;
}
