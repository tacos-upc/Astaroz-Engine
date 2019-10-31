#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"


class ModuleEditor : public Module{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void processEvent(SDL_Event);
};

