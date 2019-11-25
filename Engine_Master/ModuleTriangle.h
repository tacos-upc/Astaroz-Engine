#ifndef __ModuleTriangle_H__
#define __ModuleTriangle_H__

#include "Module.h"
#include "Globals.h"
#include "Math/MathAll.h"
#include "Application.h"
#include "ModuleProgramShader.h"
#include "ModuleTexture.h"
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

#endif __ModuleTriangle_H__