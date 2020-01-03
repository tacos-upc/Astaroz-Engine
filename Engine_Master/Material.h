#ifndef __Material_h__
#define __Material_h__

#include "Math/float4.h"
#include <list>
#include "Mesh.h"

#define MAXTEXTURES 4
#define DEFAULTMAT "Default"

enum class TextureType
{
	DIFFUSE = 0,
	SPECULAR,
	OCCLUSION,
	EMISSIVE
};

class Material
{
public:
	Material();
	Material(const Material& material);

	~Material();

	
	Texture * GetTexture(TextureType type) const;
	std::list<Texture*> GetTextures() const;

	//void SetUniforms(unsigned shader) const;

public:
	std::string name;
	

	Texture* textures[MAXTEXTURES]{ nullptr };

	float4 diffuse_color = float4::one;
	float3 specular_color = float3::one;
	float3 emissive_color = float3::one;

	float kAmbient = 0.3f;
	float kDiffuse = 0.2f;
	float kSpecular = 0.1f;
	float shininess = 32.f;

};

#endif __Material_h__