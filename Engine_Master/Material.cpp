#include "Material.h"

#include "Application.h"

#include "ModuleModelLoader.h"

#include "ModuleProgramShader.h"



Material::Material()

{

}



Material::Material(std::vector<Texture> textures, unsigned int diffuse_texture, unsigned int specular_texture, unsigned int occlusion_texture, unsigned int emissive_texture)

{

	this->textures = textures;

	this->diffuse_texture = diffuse_texture;

	this->specular_texture = specular_texture;

	this->emissive_texture = emissive_texture;

	this->occlusion_texture = occlusion_texture;

}



Material::Material(const Material & material)

{

}



Material::~Material()

{

}



void Material::SetUniforms()

{



	glUniform4f(glGetUniformLocation(App->programShader->myProgram, "material.diffuse_color"), diffuse_color.x, diffuse_color.y, diffuse_color.z, diffuse_color.w);

	glUniform3f(glGetUniformLocation(App->programShader->myProgram, "material.specular_color"), specular_color.x, specular_color.y, specular_color.z);

	glUniform3f(glGetUniformLocation(App->programShader->myProgram, "material.emissive_color"), emissive_color.x, emissive_color.y, emissive_color.z);



	glUniform1f(glGetUniformLocation(program, "material.k_ambient"), k_ambient);

	glUniform1f(glGetUniformLocation(program, "material.k_diffuse"), k_diffuse);

	glUniform1f(glGetUniformLocation(program, "material.k_specular"), k_specular);

	glUniform1f(glGetUniformLocation(program, "material.shininess"), shininess);

	glUniform3f(glGetUniformLocation(App->programShader->myProgram, "light_pos"), App->modelLoader->light_pos.x, App->modelLoader->light_pos.y, App->modelLoader->light_pos.z);


}