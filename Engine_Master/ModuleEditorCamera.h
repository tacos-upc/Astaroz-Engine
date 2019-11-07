#pragma once
#include "Module.h"
#include "Globals.h"
#include "Geometry/Frustum.h"


class ModuleEditorCamera : public Module {
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	//Frustum myFrustum;
};

