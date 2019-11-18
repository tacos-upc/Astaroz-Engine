#pragma once
#include "Module.h"
#include "Globals.h"
#include "Math/MathAll.h"
#include "Application.h"
#include "ModuleProgramShader.h"
#include "ModuleEditorCamera.h"

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

