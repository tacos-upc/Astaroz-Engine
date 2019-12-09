#include "ModuleTime.h"
#include "Math/MathAll.h"




ModuleTime::ModuleTime()
{
}


ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init()
{
	LOG("Init SDL Time");
	bool ret = true;

	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		LOG("SDL_TIMER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		millisTimer = new MsTimer();
		microTimer = new MicroTimer();

		timeScale = 1.0f;
		state = STOP;
	}

	return ret;
}

void ModuleTime::play()
{
	if (state == PLAY) return;

	state = PLAY;

	millisTimer->start();
	microTimer->start();
}

void ModuleTime::tick()
{
	if (state != PAUSE) return;
}

void ModuleTime::pause()
{
	if (state != PLAY) return;
	state = PAUSE;
}

void ModuleTime::stop()
{
	if (state == STOP) return;
	state = STOP;
}

float ModuleTime::realDeltaTime()
{
	return math::Abs(beginTimeStamp - endTimeStamp) / 1000.0f;
}

float ModuleTime::gameDeltaTime()
{
	return realDeltaTime() * timeScale;
}

float ModuleTime::realTimeSinceStartUp()
{
	return millisTimer->myStartTime;
}

void ModuleTime::setTimeScale(float scale)
{
	if (scale < 0.0f) return;
	timeScale = scale;
}

void ModuleTime::frameStart()
{
	beginTimeStamp = millisTimer->read();
}

void ModuleTime::frameEnd()
{
	endTimeStamp = millisTimer->read();
}

bool ModuleTime::Cleanup()
{
	delete(millisTimer);
	delete(microTimer);

	return true;
}
