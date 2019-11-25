#pragma once
#include "Module.h"
#include "Globals.h"

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

private:
	const Uint8 *keyboard = NULL;
	bool windowEvents[WE_COUNT];
	KeyState* keys = NULL;
};