#ifndef __ModuleTexture_H__
#define __ModuleTexture_H__

#include "Module.h"
#include "Mesh.h"

#include "IL/il.h"
#include "IL/ilut.h"
#include "IL/ilu.h"

class ModuleTexture : public Module {
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	Texture LoadTexture(const char* path);
	void loadSkybox(const char* path, int index);

public:
	//ILuint myLena;	--> Lenna exercise; improved code so not needed anymore
	//GLuint myTexture; --> Lenna exercise; improved code so not needed anymore
	//ILubyte* myData;	--> Lenna exercise; improved code so not needed anymore

	ILuint myTextureId;
	ILinfo myImageInfo;
	GLuint myTexture;
};

#endif __ModuleTexture_H__