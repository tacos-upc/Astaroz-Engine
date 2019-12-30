#ifndef _MODULE_DEBUGDRAW_H_
#define _MODULE_DEBUGDRAW_H_

#include "Module.h"
#include "ModuleEditorCamera.h"

class DDRenderInterfaceCoreGL;

class ModuleDebugDraw : public Module
{
public:

	ModuleDebugDraw();
	~ModuleDebugDraw();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

	void Draw(ModuleEditorCamera* camera, unsigned fbo, unsigned fb_width, unsigned fb_height);
private:

	static DDRenderInterfaceCoreGL* implementation;
};

#endif