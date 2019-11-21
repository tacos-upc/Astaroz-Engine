#pragma once
#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleTexture.h"
#include "ModuleEditor.h"
#include "Mesh.h"

//#include <vector>
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"

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

public:
	std::vector<Texture> texturesLoaded;
	std::vector<Mesh*> meshes;
	std::string directory;
	unsigned int numMeshes = 0;
	int numPolys = NULL;
	unsigned int numVertices = 0;
	int textureWidth;
	int textureHeight;
	char* textureType = nullptr;
	int textureId;
};
