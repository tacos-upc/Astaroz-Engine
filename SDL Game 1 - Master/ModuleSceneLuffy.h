#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleSceneLuffy : public Module
{
public:
	ModuleSceneLuffy();
	~ModuleSceneLuffy();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	SDL_Texture* texture = nullptr;
};
