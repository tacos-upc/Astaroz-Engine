#pragma once
#include "Module.h";

#include "glew.h";
#include "IL/il.h";
#include "IL/ilut.h";
#include "IL/ilu.h";

class ModuleTexture : public Module {
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	ILuint myLena;
	GLuint myTexture;
	ILubyte* myData;
};

