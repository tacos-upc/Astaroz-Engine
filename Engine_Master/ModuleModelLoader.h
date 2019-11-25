#ifndef __ModuleModelLoader_H__
#define __ModuleModelLoader_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "Mesh.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"

#include "scene.h"
#include "postprocess.h"
#include "Geometry/AABB.h"

class ModuleModelLoader : public Module
{
public:
	ModuleModelLoader();
	~ModuleModelLoader();

	bool Init();
	update_status Update();
	bool CleanUp();

	void LoadModel(const char*);
	void Draw(unsigned int program);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
	void loadNewModel(const char* path);
	void generateBoundingBox();
	void addTexture(Texture texture);

public:
	//Lists
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh*> meshes;

	//For our model
	AABB myBoundingBox;
	unsigned int numMeshes = 0;
	int numPolys = NULL;
	unsigned int numVertices = 0;

	//For textures in our model
	int textureWidth;
	int textureHeight;
	char* textureType = nullptr;
	int textureId;
};

#endif __ModuleModelLoader_H__