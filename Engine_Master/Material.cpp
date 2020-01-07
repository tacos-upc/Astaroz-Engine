#include "Material.h"


//Texture * Material::GetTexture(char* type) const
//{
//	//return textures[(unsigned)type];
//}

//std::list<Texture*> Material::GetTextures() const
//{
//	std::list<Texture*> mytextures;
//	for (unsigned i = 0; i < MAXTEXTURES; i++)
//	{
//		if (textures[i] != nullptr)
//		{
//			mytextures.push_back(textures[i]);
//		}
//	}
//	return mytextures;
//}

Material::Material()
{
}

Material::Material(const Material & material)
{
}

Material::~Material()
{
}

void Material::SetUniforms() const
{
	for (unsigned int i = 0; i < 4; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		char* textureType = nullptr;
		float* color = (float*)&float3::zero;
		if (textures[i].type == "diffuse") {
			textureType = "diffuse";
			color = (float*)&diffuse_color;
		}
		else if (textures[i].type == "texture_specular") {
			textureType = "specular";
			color = (float*)&specular_color;
		}
		else if (textures[i].type == "texture_occlusion") {
			textureType = "occlusion";
		}
		else if (textures[i].type == "texture_emissive"){
			textureType = "emissive";
			color = (float*)&emissive_color;
		}

		char texture[32];
		sprintf(texture, "material.%s_texture", textureType);

		char uniform[32];
		sprintf(uniform, "material.%s_color", textureType);

		if (1==1)
		{
			if (textures[i].type== "texture_diffuse")
			{
				glUniform4fv(glGetUniformLocation(program,
					uniform), 1, color);
			}
			else
			{
				glUniform3fv(glGetUniformLocation(program,
					uniform), 1, color);
			}
			glBindTexture(GL_TEXTURE_2D, textures[i].id);

			glUniform1i(glGetUniformLocation(program, texture), i);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(glGetUniformLocation(program, texture), i);
			float3 noColor = float3::zero; //Used as a fallback

			glUniform3fv(glGetUniformLocation(program,
				uniform), 1, (GLfloat*)&noColor);
		}
		glDisable(GL_TEXTURE_2D);
	}

	glUniform1fv(glGetUniformLocation(program,
		"material.k_ambient"), 1, (GLfloat*)&k_ambient);
	glUniform1fv(glGetUniformLocation(program,
		"material.k_diffuse"), 1, (GLfloat*)&k_diffuse);
	glUniform1fv(glGetUniformLocation(program,
		"material.k_specular"), 1, (GLfloat*)&k_specular);
	glUniform1fv(glGetUniformLocation(program,
		"material.shininess"), 1, (GLfloat*)&shininess);
}
