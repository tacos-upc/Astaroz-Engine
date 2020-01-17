#ifndef __MODEL_H__
#define __MODEL_H__

#include "Globals.h"
#include "Mesh.h"
#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include "Logger.hpp"
#include "LogStream.hpp"
#include "MathGeoLib.h"
#include "Geometry/AABB.h"
#include "ModuleTexture.h"
#include <string>
#include "Material.h"

using namespace std;

class Model {

public:

	Model(const char *path);
	~Model();

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
	std::vector<Material*> materials;

	//General
	std::string modelName;
	int numTextures;

	//Lists
	std::vector<Texture*> texturesLoaded;
	std::vector<Mesh*> meshes;

	//scale
	math::float3 scale = float3(0.05f, 0.05f, 0.05f);

private:

	void LoadModel(const char*);
	std::vector<Texture*> loadMaterialTextures(aiMaterial*, aiTextureType, char*);
	void addTexture(Texture* texture);
	void DrawModel(unsigned int program);
	void processNode(aiNode*, const aiScene*, const char* path);
	Mesh processMesh(aiMesh*, const aiScene*, const char* path);
	void generateBoundingBox();



};
#endif // __MODEL_H__