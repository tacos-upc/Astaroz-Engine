#ifndef __Material_h__
#define __Material_h__

#include "Math/float4.h"
#include <list>
#include "Mesh.h"


#define DEFAULTMAT "Default"

class Material
{
public:
	Material();
	Material(std::vector<Texture*> textures,unsigned int diffuse_texture, unsigned int specular_texture, unsigned int occlusion_texture ,unsigned int emissive_texture);
	Material(const Material& material);

	~Material();
	

    void SetUniforms();

	std::string name;
	unsigned int program = 0;
	math::float4 object_color = math::float4::zero;
	
	std::vector<Texture*> textures;

	float4 diffuse_color = float4::one;
	float3 specular_color = float3::one;
	float3 emissive_color = float3::one;
	float k_ambient = 0.5f;
	float k_diffuse = 0.4f;
	float k_specular = 0.4f;
	float shininess = 0.2f;

	unsigned int diffuse_texture;
	unsigned int specular_texture;
	unsigned int occlusion_texture;
	unsigned int emissive_texture;

	bool hasdiff = false;
	bool hasSpec = false;
	bool hasEmi = false;
	bool hasOcc = false;
};

#endif __Material_h__
