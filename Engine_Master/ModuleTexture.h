#pragma once
#include "Module.h";
#include "Mesh.h"

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

	Texture ModuleTexture::LoadTexture(const char* path);

public:
	//ILuint myLena;	--> Lenna exercise; improved code so not needed anymore
	//GLuint myTexture; --> Lenna exercise; improved code so not needed anymore
	//ILubyte* myData;	--> Lenna exercise; improved code so not needed anymore

	ILuint myTextureId;
	ILinfo myImageInfo;
	GLuint myTexture;
};

