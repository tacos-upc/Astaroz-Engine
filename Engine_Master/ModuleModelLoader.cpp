#include "ModuleModelLoader.h"
#include "ModuleEditorCamera.h"

#include "cimport.h"
#include "Importer.hpp"

//Used on the assimp log debugger
void addLog(const char* str, char* userData)
{
	std::string info(str);
	info.pop_back();

	//Use our LOG now
	LOG(info.c_str());
}

ModuleModelLoader::ModuleModelLoader()
{}

ModuleModelLoader::~ModuleModelLoader()
{}

bool ModuleModelLoader::Init()
{
	//LOG loading process from ASSIMP
	aiLogStream sLog = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, NULL);
	sLog.callback = addLog;
	aiAttachLogStream(&sLog);

	//Always start by loading the Baker house model
	LoadModel(MODEL_BAKER_PATH);

	return true;
}

update_status ModuleModelLoader::Update()
{
	return UPDATE_CONTINUE;
}

bool ModuleModelLoader::CleanUp()
{
	return true;
}

void ModuleModelLoader::Draw(unsigned int program)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i]->Draw(program);
}

void ModuleModelLoader::LoadModel(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
		return;
	}
	else {
		LOG("Path of the geometry correct.\n");
	}
	//Next step
	processNode(scene->mRootNode, scene);

	//Fill AABB member value
	generateBoundingBox();

	//Center camera to new model
	//App->editorCamera->focusModel();
}

void ModuleModelLoader::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(new Mesh(processMesh(mesh, scene)));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh ModuleModelLoader::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		float3 vector;

		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		numPolys = (vertices.size()) / 3;
		// normals
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// texture coordinates
		if (mesh->mTextureCoords[0])
		{
			float2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = float2(0.0f, 0.0f);

		// tangent
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;

		// bitangent
		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.Bitangent = vector;
		vertices.push_back(vertex);
	}

	for (unsigned int i=0; i<mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j=0; j<face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// 1. diffuse maps
	std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	if (textures[0].type == "texture_diffuse") {
		textureType = textures[0].type;
	}
	// 2. specular maps
	std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	if (textures[0].type == "texture_specular") {
		textureType = textures[0].type;
	}
	// 3. normal maps
	std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	if (textures[0].type == "texture_normal") {
		textureType = textures[0].type;
	}
	// 4. height maps
	std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	textureWidth = textures[0].width;
	textureHeight = textures[0].height;
	textureId = textures[0].id;
	numPolys /= 3;

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModuleModelLoader::loadMaterialTextures(aiMaterial *mat, aiTextureType type, char* typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i=0; i<mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		//This flag will be used to know if the texture was previously loaded or not
		bool skip = false;
		for (unsigned int j=0; j<texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j].path, str.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true; //enable this flag to not load the texture again from the file
				break;
			}
		}
		//if texture hasn't been loaded already, load it
		if (!skip)
		{
			Texture texture = App->texture->LoadTexture(str.C_Str());
			texture.type = typeName;
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

void ModuleModelLoader::loadNewModel(const char* path)
{
	//Clear lists from member variables
	texturesLoaded.clear();
	meshes.clear();

	//Load model
	LoadModel(path);
}

void ModuleModelLoader::generateBoundingBox()
{
	float3 min, max;
	min = {100000.f, 100000.f, 100000.f};		//reference value to be replaced
	max = {-100000.f, -100000.f, -100000.f};	//reference value to be replaced
	for (auto mesh : meshes)
	{
		for (auto vertex : mesh->vertices)
		{
			//min
			min.x = MIN(min.x, vertex.Position.x);
			min.y = MIN(min.y, vertex.Position.y);
			min.z = MIN(min.z, vertex.Position.z);

			//max
			max.x = MAX(max.x, vertex.Position.x);
			max.y = MAX(max.y, vertex.Position.y);
			max.z = MAX(max.z, vertex.Position.z);
		}
	}
	myBoundingBox.minPoint = min;
	myBoundingBox.maxPoint = max;
}

void ModuleModelLoader::addTexture(Texture texture)
{
	texturesLoaded.push_back(texture);
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->updateTexture(texture);
	}
}