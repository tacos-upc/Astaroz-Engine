#ifndef __Mesh_H__
#define __Mesh_H__

#include "Globals.h"
#include "Module.h"
#include <string>
#include <vector>
#include "Math/float2.h"
#include "Math/float3.h"

//Structs
struct Texture
{
	unsigned int id;
	char*  type;
	const char* path;
	int width;
	int height;
	unsigned char *data;
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
	std::vector<Vertex> vertices;
	std::vector<Triangle> triangles;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	const char* name;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures, const char* name);
	~Mesh();

	void Init();
	void Draw(unsigned int program) const;
	void updateTexture(Texture texture);

	void updateTriangles();

private:
	unsigned int VAO = 0, VBO = 0, EBO = 0;
	void setupMesh();
};

#endif __Mesh_H__