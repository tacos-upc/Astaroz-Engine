#ifndef __ModuleInput_H__
#define __ModuleInput_H__

#include "Module.h"
#include "Globals.h"
#include "Point.h"

#define NUM_MOUSE_BUTTONS 5

typedef unsigned __int8 Uint8;

class ModuleInput : public Module
{
public:
	ModuleInput();
	~ModuleInput();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();

	bool getWindowEvent(EventWindow code) const;
	KeyState getKey(int id) const;
	bool isKeyDown(int id) const;

	bool GetMouseButtonDown(int id) const
	{
		return mouseButtons[id - 1] == KEY_DOWN || mouseButtons[id - 1] == KEY_REPEAT;
	}

	bool GetMouseButtonUp(int id) const
	{
		return mouseButtons[id - 1] == KEY_UP;
	}

	float getWheelSpeed()
	{
		//LOG("%f", mouseWheel);
		return mouseWheel;
	}

	// Get mouse / axis position
	const fPoint& GetMouseMotion() const;
	const iPoint& GetMousePosition() const;

private:
	const Uint8 *keyboard = NULL;
	bool windowEvents[WE_COUNT];
	KeyState* keys = NULL;
	KeyState mouseButtons[NUM_MOUSE_BUTTONS];
	fPoint mouseMotion;
	iPoint mouse;

	float mouseWheel = 0.0f;
};

#endif __ModuleInput_H__