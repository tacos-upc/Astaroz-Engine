#include "Material.h"


Material::Material()
{
	diffuse_color = float4::one;
	specular_color = float3::one;
	emissive_color = float3::one;
	k_ambient = 0.3f;
	k_diffuse = 0.2f;
	k_specular = 0.1f;
	shininess = 32.f;
}

Material::Material(const Material & material)
{
}

Material::~Material()
{
}

void Material::SetUniforms() const
{
	for (unsigned int i = 0; i < textures.size(); i++)
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
		else if (textures[i].type == "texture_emissive") {
			textureType = "emissive";
			color = (float*)&emissive_color;
		}

		char texture[32];
		sprintf(texture, "material.%s_texture", textureType);

		char uniform[32];
		sprintf(uniform, "material.%s_color", textureType);


		if (textures[i].type == "texture_diffuse")
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

	glUniform1fv(glGetUniformLocation(program,
		"material.k_ambient"), 1, (GLfloat*)&k_ambient);
	glUniform1fv(glGetUniformLocation(program,
		"material.k_diffuse"), 1, (GLfloat*)&k_diffuse);
	glUniform1fv(glGetUniformLocation(program,
		"material.k_specular"), 1, (GLfloat*)&k_specular);
	glUniform1fv(glGetUniformLocation(program,
		"material.shininess"), 1, (GLfloat*)&shininess);
}
