#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Module.h"
#include <string>
#include <vector>
#include "Math/float2.h"
#include "Math/float3.h"
#include "Math/float4x4.h"

//Structs
/*struct Texture
{
	unsigned int id;
	char*  type;
	const char* path;
	int width;
	int height;
	unsigned char *data;
};
*/
struct Texture
{
	unsigned char *data;
	char*  type;
	const char* path;
	unsigned id = 0;
	unsigned width = 0;
	unsigned height = 0;
	
	Texture() 
	{}

	Texture(const char* file) : path(file)
	{}

	~Texture();
};
struct Vertex
{
	float3 Position;
	float3 Normal;
	float2 TexCoords;
	float3 Tangent;
	float3 Bitangent;
};

//Class
class Mesh
{
public:
	/*  Mesh Data  */

	const char* name;
	math::float4x4  transform = math::float4x4::identity;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;
	

	Mesh();
	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures, unsigned int material);

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures, const char* name);
	~Mesh();

	void Init();
	void Draw(unsigned int program) const;
	void updateTexture(Texture* texture);
	void setUniforms();
	unsigned int material;


	unsigned int VAO = 0, VBO = 0, EBO = 0;
	unsigned normals_offset = 0;
	unsigned vertex_size = 0;
	unsigned num_vertices = 0;
	unsigned num_indices = 0;
	unsigned texcoords_offset = 0;
	bool hastext = true;
	void setupMesh();
};

#endif __Mesh_H__