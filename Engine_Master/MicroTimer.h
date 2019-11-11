#pragma once
#include "Globals.h"
#include "SDL/include/SDL.h"


class MicroTimer {
public:
	MicroTimer();
	~MicroTimer();

	void start();
	Uint32 read();
	void stop();

public:
	Uint32 myStartTime, myCurrentTime;
	bool running;
};


