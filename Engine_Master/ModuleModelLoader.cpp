#include "ModuleModelLoader.h"
#include "ModuleEditorCamera.h"
#include "ModuleProgramShader.h"
#include "Mesh.h"
#include "cimport.h"
#include "Importer.hpp"

#define PAR_SHAPES_IMPLEMENTATION
#include "Util/par_shapes.h"
#pragma warning(pop)



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
	//LoadModel(MODEL_BAKER_PATH);

	LoadSphere("sphere0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	LoadSphere("sphere1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	LoadSphere("sphere2", math::float3(8.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	LoadSphere("sphere3", math::float3(11.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));


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
	App->editorCamera->focusModel();
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

bool ModuleModelLoader::LoadSphere(const char* name, const math::float3& pos, const math::Quat& rot, float size,
	unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_parametric_sphere(int(slices), int(stacks));

	if (mesh)
	{
		par_shapes_scale(mesh, size, size, size);

		GenerateMesh(name, pos, rot, mesh);
		par_shapes_free_mesh(mesh);

		meshes.back()->material = materials.size();
		

		Material mat;
		mat.program = App->programShader->myProgram;
		mat.object_color = color;

		materials.push_back(mat);

		return true;
	}

	return false;
}


bool ModuleModelLoader::LoadTorus(const char* name, const math::float3& pos, const math::Quat& rot, float inner_r, float outer_r,
	unsigned slices, unsigned stacks, const math::float4& color)
{
	par_shapes_mesh* mesh = par_shapes_create_torus(int(slices), int(stacks), inner_r);

	if (mesh)
	{
		par_shapes_scale(mesh, outer_r, outer_r, outer_r);
		GenerateMesh(name, pos, rot, mesh);
		par_shapes_free_mesh(mesh);

		
		meshes.back()->material = materials.size();
		
		Material mat;
		mat.program = App->programShader->myProgram;
		mat.object_color = color;

		materials.push_back(mat);

		return true;
	}

	return false;
}


void ModuleModelLoader::GenerateMesh(const char* name, const math::float3& pos, const math::Quat& rot, par_shapes_mesh* shape)
{
	Mesh* dst_mesh= new Mesh();

	dst_mesh->name = name;
	dst_mesh->transform = math::float4x4(rot, pos);

	glGenBuffers(1, &dst_mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, dst_mesh->VBO);

	// Positions

	for (unsigned i = 0; i< unsigned(shape->npoints); ++i)
	{
		math::float3 point(shape->points[i * 3], shape->points[i * 3 + 1], shape->points[i * 3 + 2]);
		point = dst_mesh->transform.TransformPos(point);
		for (unsigned j = 0; j < 3; ++j)
		{
			min_v[j] = min(min_v[j], point[i]);
			max_v[j] = max(max_v[j], point[i]);
		}
	}

	unsigned offset_acc = sizeof(math::float3);

	if (shape->normals)
	{
		dst_mesh->normals_offset = offset_acc;
		offset_acc += sizeof(math::float3);
	}

	dst_mesh->vertex_size = offset_acc;

	glBufferData(GL_ARRAY_BUFFER, dst_mesh->vertex_size*shape->npoints, nullptr, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(math::float3)*shape->npoints, shape->points);

	// normals

	if (shape->normals)
	{
		glBufferSubData(GL_ARRAY_BUFFER, dst_mesh->normals_offset*shape->npoints, sizeof(math::float3)*shape->npoints, shape->normals);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// indices

	glGenBuffers(1, &dst_mesh->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh->EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned)*shape->ntriangles * 3, nullptr, GL_STATIC_DRAW);

	unsigned* indices = (unsigned*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0,
		sizeof(unsigned)*shape->ntriangles * 3, GL_MAP_WRITE_BIT);

	for (unsigned i = 0; i< unsigned(shape->ntriangles * 3); ++i)
	{
		*(indices++) = shape->triangles[i];
	}

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	dst_mesh->material = 0;
	dst_mesh->num_vertices = shape->npoints;
	dst_mesh->num_indices = shape->ntriangles * 3;

	//generate VAO

	glGenVertexArrays(1, &dst_mesh->VAO);

	glBindVertexArray(dst_mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, dst_mesh->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst_mesh->EBO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	if (dst_mesh->normals_offset != 0)
	{
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(dst_mesh->normals_offset*dst_mesh->num_vertices));
	}

	if (dst_mesh->texcoords_offset != 0)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(dst_mesh->texcoords_offset*dst_mesh->num_vertices));
	}

	glBindVertexArray(0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	meshes.push_back(dst_mesh);
	dst_mesh->hastext = false;

	bsphere.center = (max_v + min_v)*0.5f;
	bsphere.radius = (max_v - min_v).Length()*0.5f;
}

