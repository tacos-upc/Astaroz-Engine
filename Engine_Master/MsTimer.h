#ifndef __MsTimer_H__
#define __MsTimer_H__

#include "Globals.h"
#include "SDL/include/SDL.h"


class MsTimer {
public:
	MsTimer();
	~MsTimer();

	void start();
	Uint32 read();
	void stop();

public:
	Uint32 myStartTime, myCurrentTime;
	bool running;

private:

	void setupCurrentTime();
};

#endif __MsTimer_H__