#include "Model.h"
#include "Application.h"
#include "ModuleProgramShader.h"
#include "ModuleModelLoader.h"

Model::Model(const char * path, float3 scale)
{
	this->scale = scale;
	LoadModel(path);
}

Model::~Model()
{
}

void Model::LoadModel(const char* path)
{
	
	Assimp::Importer importer;
	std::string fullPath = path;
	fullPath += FBX;
	fullPath = MODELS_PATH + fullPath;
	const char *fbxPath = fullPath.c_str();
	
	const aiScene* scene = importer.ReadFile(fbxPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG("ERROR::ASSIMP:: %s\n", importer.GetErrorString());
		return;
	}
	else {
		LOG("Path of the geometry correct.\n");
	}


	//Next step	
	scene->mNumMeshes;
	processNode(scene->mRootNode, scene, path);

	//Fill AABB member value
	generateBoundingBox();

	//Center camera to new model
	//App->editorCamera->focusModel();

	//Take the model's name from path
	modelName = path; //TODO --> extract name from path and not the full route
}

void Model::processNode(aiNode *node, const aiScene *scene, const char* path)
{

	LOG("Before Meshes: %d", meshes.size());
	LOG("------------------------------------------")
		
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{

			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(new Mesh(processMesh(mesh, scene, path)));
			LOG("After Meshes: %d", meshes.size());
			LOG("------------------------------------------");
		}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, path);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, const char* path)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		float3 vector;

		// positions
		vector.x = mesh->mVertices[i].x*scale.x;
		vector.y = mesh->mVertices[i].y*scale.y;
		vector.z = mesh->mVertices[i].z*scale.z;
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

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	if (path == MESH_PLAYER) {
		if (meshes.size() == 0) {
			path = MESH_GUN;
		}
	}

	std::string diffusePath = path;
	diffusePath += DIFFUSE;
	diffusePath += PNG;
	const char *dPath = diffusePath.c_str();
	Texture texture = App->texture->LoadTexture(dPath);
	texture.type = "diffuse";

	textures.push_back(&texture);
	texturesLoaded.push_back(&texture);

	std::string specularPath = path;
	specularPath += SPECULAR;
	specularPath += TIF;
	const char *sPath = specularPath.c_str();
	Texture texture2 = App->texture->LoadTexture(sPath);
	texture2.type = "specular";
	textures.push_back(&texture2);
	texturesLoaded.push_back(&texture2);

	std::string emissivePath = path;
	emissivePath += EMISSIVE;
	emissivePath += PNG;
	const char *ePath = emissivePath.c_str();
	Texture texture3 = App->texture->LoadTexture(ePath);
	texture3.type = "emissive";
	textures.push_back(&texture3);
	texturesLoaded.push_back(&texture3);

	std::string occlusionPath = path;
	occlusionPath += OCCLUSION;
	occlusionPath += PNG;
	const char *oPath = occlusionPath.c_str();
	Texture texture4 = App->texture->LoadTexture(oPath);
	texture4.type = "occlusion";
	textures.push_back(&texture4);
	texturesLoaded.push_back(&texture4);

	Material* myMaterial = new Material(textures, texture.id, texture2.id, texture4.id, texture3.id);
	myMaterial->hasdiff = texture.loaded;
	myMaterial->hasSpec = texture2.loaded;
	myMaterial->hasEmi = texture3.loaded;
	myMaterial->hasOcc = texture4.loaded;
	myMaterial->program = App->programShader->defaultProgram;
	materials.push_back(myMaterial);
	App->modelLoader->materials.push_back(myMaterial);

	//// 1. diffuse maps
	//std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
	//textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//if (textures[0].type == "texture_diffuse") {
	//	textureType = textures[0].type;
	//}
	//if (diffuseMaps.size() > 0) {
	//	finmaterial->textures.insert(finmaterial->textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//}
	////difuse color
	//
	//aiColor4D diffuse;
	//aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	//finmaterial->diffuse_color[0] = diffuse.r;
	//finmaterial->diffuse_color[1] = diffuse.g;
	//finmaterial->diffuse_color[2] = diffuse.b;
	//finmaterial->diffuse_color[3] = diffuse.a;
	//// 2. specular maps
	//std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
	//textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//if (textures[0].type == "texture_specular") {
	//	textureType = textures[0].type;
	//}
	//if (specularMaps.size() > 0) {
	//	finmaterial->textures.insert(finmaterial->textures.end(), specularMaps.begin(), specularMaps.end());
	//}
	////specular color
	//aiColor4D specular;
	//aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
	//finmaterial->specular_color[0] = specular.r;
	//finmaterial->specular_color[1] = specular.g;
	//finmaterial->specular_color[2] = specular.b;
	//
	//
	//// 3. normal maps
	//std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
	//textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//if (textures[0].type == "texture_normal") {
	//	textureType = textures[0].type;
	//}
	//if (normalMaps.size() > 0) {
	//	finmaterial->textures.insert(finmaterial->textures.end(), normalMaps.begin(), normalMaps.end());
	//}
	//// 4. height maps
	//std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
	//textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	//if (heightMaps.size() > 0) {
	//	finmaterial->textures.insert(finmaterial->textures.end(), heightMaps.begin(), heightMaps.end());
	//}
	//
	//std::vector<Texture> emissiveMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
	//textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
	//if (textures[0].type == "texture_normal") {
	//	textureType = textures[0].type;
	//}
	//if (emissiveMaps.size() > 0) {
	//	finmaterial->textures.insert(finmaterial->textures.end(), emissiveMaps.begin(), emissiveMaps.end());
	//}
	//
	//
	//

	const char *meshName = path;
	unsigned int aux = App->modelLoader->materials.size() - 1;
	return Mesh(vertices, indices, textures, aux, meshName);
}

std::vector<Texture*> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, char* typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		//This flag will be used to know if the texture was previously loaded or not
		bool skip = false;
		for (unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if (std::strcmp(texturesLoaded[j]->path, str.C_Str()) == 0)
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
			textures.push_back(&texture);
			texturesLoaded.push_back(&texture);
		}
	}
	return textures;
}

void Model::generateBoundingBox()
{
	float3 min, max;
	min = { 100000.f, 100000.f, 100000.f };		//reference value to be replaced
	max = { -100000.f, -100000.f, -100000.f };	//reference value to be replaced
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

void Model::addTexture(Texture* texture)
{
	texturesLoaded.push_back(texture);
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		(*it)->updateTexture(texture);
	}
}

void Model::DrawModel(unsigned int program)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i]->setUniforms();
		meshes[i]->Draw(program);
	}
}