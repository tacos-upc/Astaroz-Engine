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
	void Material::SetUniforms() const;
	
	//Texture * GetTexture(char* type) const;
	//std::list<Texture*> GetTextures() const;

	//void SetUniforms(unsigned shader) const;

public:
	std::string name;
	unsigned program = 0;
	math::float4 object_color = math::float4::zero;
	
	std::vector<Texture> textures;

	float4 diffuse_color = float4::one;
	float3 specular_color = float3::one;
	float3 emissive_color = float3::one;

	float k_ambient = 0.3f;
	float k_diffuse = 0.2f;
	float k_specular = 0.1f;
	float shininess = 32.f;

};

#endif __Material_h__
