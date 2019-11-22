#pragma once

#include "Globals.h"
#include "Module.h"
#include <string>
#include <vector>
#include "Math/float2.h"
#include "Math/float3.h"

struct Texture {
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

class Mesh
{
public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
	~Mesh();
	/*  Functions  */
	void Init();

	void Draw(unsigned int program) const;
private:
	/*  Render data  */
	unsigned int VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();
};
