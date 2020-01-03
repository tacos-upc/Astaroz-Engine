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
#include "Math/Quat.h"

struct par_shapes_mesh_s;

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

	bool LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size, unsigned slices, unsigned stacks, const math::float4& color);
	bool LoadTorus(const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r, unsigned slices, unsigned stacks, const math::float4& color);
	void GenerateMesh(const char * name, const math::float3 & pos, const math::Quat & rot, par_shapes_mesh_s * shape);
	
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
	struct Material
	{
		unsigned program = 0;
		unsigned diffuse_map = 0;
		math::float3 diffuse;
		math::float3 specular;
		math::float3 ambient;
		math::float4 object_color = math::float4::zero;
		float shininess = 0.0f;
		float k_specular = 0.0f;
		float k_diffuse = 0.0f;
		float k_ambient = 0.0f;
	};
	std::vector<Material> materials;
	math::float3 min_v = math::float3(FLT_MAX, FLT_MAX, FLT_MAX);
	math::float3 max_v = math::float3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	struct Sphere
	{
		math::float3 center = math::float3::zero;
		float        radius = 0.0f;
	};
	

	math::float3 light_pos = math::float3(1.0f, 1.0f, 1.0f);
	float ambient = 0.3f;
	Sphere bsphere;
};

#endif __ModuleModelLoader_H__