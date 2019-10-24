#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"

class ModuleTriangle :	public Module{
public:
	ModuleTriangle();
	virtual ~ModuleTriangle();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();

private:
	GLuint vbo;
};

