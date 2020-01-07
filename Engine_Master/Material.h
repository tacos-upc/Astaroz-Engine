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
	Material(const Material& material);

	~Material();
	

    void SetUniforms() const;

public:
	std::string name;
	unsigned program = 0;
	math::float4 object_color = math::float4::zero;
	
	std::vector<Texture> textures;

	float4 diffuse_color;
	float3 specular_color;
	float3 emissive_color;

	float k_ambient;
	float k_diffuse;
	float k_specular;
	float shininess;

};

#endif __Material_h__
