#ifndef __ModuleTime_H__
#define __ModuleTime_H__


#pragma once
#include "Module.h"
#include "SDL.h"
#include "MsTimer.h"
#include "MicroTimer.h"
#include "ModuleEditor.h"

enum TimeState { PLAY, PAUSE, STOP };

class ModuleTime :
	public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Init();

	update_status Update();

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
	
	float realTimeBeginTimeStamp;
	float realTimeEndTimeStamp;

	float gameTimeBeginTimeStamp;
	float gameTimeEndTimeStamp;

	float timeSinceStartUp;
	float framesSinceStartUp;
	
	float timeSinceGameStart;
	float framesSinceGameStart;
	
	TimeState state;
};

#endif __ModuleTime_H__
