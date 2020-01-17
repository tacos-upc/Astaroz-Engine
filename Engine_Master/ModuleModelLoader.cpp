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

	light = Light();


	return true;
}

bool ModuleModelLoader::Start()
{
	//Charge all the models
	//AddModel(MODEL_BAKER_PATH);

	//Assigment models

	AddModel(MODEL_BUNNY, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_CLOCK, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_DOLLHOUSE, float3(0.005f, 0.005f, 0.005));
	AddModel(MODEL_DRAWERS, float3(0.005f, 0.005f, 0.005));
	AddModel(MODEL_FIRETRUCK, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_FLOOR, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_HEARSE, float3(0.01f, 0.01f, 0.01));
	AddModel(MODEL_PLAYER, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_ROBOT, float3(0.01f, 0.01f, 0.01f));
	AddModel(MODEL_WALL, float3(0.05f, 0.05f, 0.05));
	AddModel(MODEL_SPINNINGTOP, float3(0.05f, 0.05f, 0.05));

	//LoadSphere("sphere0", math::float3(2.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	//materials.back().k_specular = 0.9f;
	//materials.back().shininess = 64.0f;
	//materials.back().k_diffuse = 0.5f;
	//materials.back().k_ambient = 1.0f;
	//
	//LoadSphere("sphere1", math::float3(5.0f, 2.0f, 0.0f), math::Quat::identity, 1.0f, 30, 30, float4(1.0f, 1.0f, 1.0f, 1.0f));
	//materials.back().k_specular = 0.9f;
	//materials.back().shininess = 20.0f;
	//materials.back().k_diffuse = 0.5f;
	//materials.back().k_ambient = 1.0f;

	//Init variables
	numMeshes = 0;
	numVertices = 0;
	numTextures = 0;
	numPolys = NULL;

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
		mat.program = App->programShader->defaultProgram;
		mat.object_color = color;

		materials.push_back(&mat);

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
		mat.program = App->programShader->defaultProgram;
		mat.object_color = color;

		materials.push_back(&mat);

		return true;
	}

	return false;
}

void ModuleModelLoader::AddModel(const char * path, math::float3 scale)
{
	Model* myNewModel = new Model(path, scale);
	for (int i = 0; i < myNewModel->meshes.size(); i++) {
		myNewModel->meshes.at(i)->scale = scale;
		meshes.push_back(myNewModel->meshes.at(i));
		
	}
	models.push_back(myNewModel);
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

void ModuleModelLoader::emptyScene()
{
	meshes.clear();
	numTextures = 0;
}
