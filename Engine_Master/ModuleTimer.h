#pragma once
#include "Module.h"


enum TimeState
{
	TIME_PLAY,
	TIME_PAUSE,
	TIME_STOP
};

class ModuleTimer :
	public Module
{
public:
	ModuleTimer();
	~ModuleTimer();

	bool Init();

	void play();
	void pause();
	void stop();

	void moveFramesForward(int);
	void moveFramesBackward(int);

	void Cleanup();

	void beginFrame();
	void endFrame();

private:
	float framesSinceStartUp;
	float timeSinceStartUpInSeconds;

	float framesSinceGamestart;
	float realTimeSinceGameStart;
	float gameTimeSinceGameStart;

	float realDeltaTime;
	float gameDeltaTime;

	float timeScale;

	TimeState timeState;
};

