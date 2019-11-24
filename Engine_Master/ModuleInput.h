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

	KeyState GetKey(int id) const
	{
		return keys[id];
	}

	bool GetWindowEvent(EventWindow code) const;

	//Returns our keyboard private member value
	const Uint8* getKeyboard();

private:
	const Uint8 *keyboard = NULL;
	bool windowEvents[WE_COUNT];
	KeyState* keys = NULL;
};