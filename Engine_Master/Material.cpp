#include "Material.h"


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
	
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		char* textureType = nullptr;
		float* color = (float*)&float3::zero;
		if (textures[i]->type == "diffuse") {
			textureType = "diffuse";
			color = (float*)&diffuse_color;
		}
		else if (textures[i]->type == "specular") {
			textureType = "specular";
			color = (float*)&specular_color;
		}
		else if (textures[i]->type == "occlusion") {
			textureType = "occlusion";
		}
		else if (textures[i]->type == "emissive") {
			textureType = "emissive";
			color = (float*)&emissive_color;
		}

		char texture[32];
		sprintf(texture, "material.%s_texture", textureType);

		char uniform[32];
		sprintf(uniform, "material.%s_color", textureType);


		if (textures[i]->type == "diffuse")
		{
			glUniform4fv(glGetUniformLocation(program,
				uniform), 1, color);
		}
		else if(textures[i]->type != "occlusion")
		{
			glUniform3fv(glGetUniformLocation(program,
				uniform), 1, color);
		}
		glUniform1i(glGetUniformLocation(program, texture), i);
		glBindTexture(GL_TEXTURE_2D, textures[i]->id);

		

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
