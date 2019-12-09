#pragma once
#include "Module.h"
#include "SDL.h"
#include "MsTimer.h"
#include "MicroTimer.h"

enum TimeState { PLAY, PAUSE, STOP };

class ModuleTime :
	public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Init();

	void play();
	void tick();
	void pause();
	void stop();

	float realDeltaTime();
	float gameDeltaTime();
	float realTimeSinceStartUp();
	void setTimeScale(float);

	void frameStart();
	void frameEnd();

	bool Cleanup();

private:
	float timeScale;
	float beginTimeStamp;
	float endTimeStamp;
	TimeState state;

	MsTimer* millisTimer;
	MicroTimer* microTimer;
};

