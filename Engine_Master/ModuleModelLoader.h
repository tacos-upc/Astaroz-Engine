#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status Update();
	bool CleanUp();
};
