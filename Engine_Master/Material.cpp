#include "Material.h"


Texture * Material::GetTexture(TextureType type) const
{
	return textures[(unsigned)type];
}

std::list<Texture*> Material::GetTextures() const
{
	std::list<Texture*> mytextures;
	for (unsigned i = 0; i < MAXTEXTURES; i++)
	{
		if (textures[i] != nullptr)
		{
			mytextures.push_back(textures[i]);
		}
	}
	return mytextures;
}