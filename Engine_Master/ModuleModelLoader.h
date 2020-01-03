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
	bool Start();
	update_status Update();
	bool CleanUp();

	void LoadModel(const char*);
	void DrawAll(unsigned int program);
	void processNode(aiNode*, const aiScene*);
	Mesh processMesh(aiMesh*, const aiScene*);
	std::vector<Texture> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
	void loadNewModel(const char* path);
	void generateBoundingBox();
	void addTexture(Texture texture);
	void emptyScene();

public:
	//Lists
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh*> meshes;

	//For our model
	AABB myBoundingBox;
	unsigned int numMeshes;
	int numPolys;
	unsigned int numVertices;

	//For textures in our model
	int textureWidth;
	int textureHeight;
	char* textureType = nullptr;
	int textureId;

	//General
	std::string modelName;
	int numTextures;
};

#endif __ModuleModelLoader_H__