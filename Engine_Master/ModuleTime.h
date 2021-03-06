#ifndef __ModuleTime_H__
#define __ModuleTime_H__


#pragma once
#include "Module.h"
#include "SDL.h"
#include "MsTimer.h"
#include "MicroTimer.h"
#include "ModuleEditor.h"
#define MIN_TIMESCALE 0.0f
#define MAX_TIMESCALE 5.0f

enum TimeState { PLAY, PAUSE, TICK };



class ModuleTime :
	public Module
{
public:
	ModuleTime();
	~ModuleTime();

	bool Init();

	update_status Update();

	void play();
	void pause();
	void tick();

	float realTimeSinceStartUp();
	void setTimeScale(float);

	float getDeltaTime();
	float getGameTimeSinceStartup();

	void frameStart();
	void frameEnd();

	bool Cleanup();

	void drawTimeData();


private:
	float timeScale;
	
	float realTimeBeginTimeStamp;
	float realTimeEndTimeStamp;
	float realDeltaTime;

	float gameTimeBeginTimeStamp;
	float gameTimeEndTimeStamp;
	float gameDeltaTime;

	float timeSinceStartUp;
	float framesSinceStartUp;
	
	float timeSinceGameStart;
	float framesSinceGameStart;
	
	TimeState state;

	void drawTimeControls();
	const char* stateToString();
};

#endif __ModuleTime_H__
