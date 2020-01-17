#include "Material.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"

Material::Material()
{
}

Material::Material(std::vector<Texture*> textures, unsigned int diffuse_texture, unsigned int specular_texture, unsigned int occlusion_texture, unsigned int emissive_texture)
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
	
	glUniform4f(glGetUniformLocation(App->programShader->defaultProgram, "material.diffuse_color"), diffuse_color.x, diffuse_color.y, diffuse_color.z, diffuse_color.w);
	glUniform3f(glGetUniformLocation(App->programShader->defaultProgram, "material.specular_color"), specular_color.x, specular_color.y, specular_color.z);
	glUniform3f(glGetUniformLocation(App->programShader->defaultProgram, "material.emissive_color"), emissive_color.x, emissive_color.y, emissive_color.z);

	glUniform1f(glGetUniformLocation(program,"material.k_ambient"), k_ambient);
	glUniform1f(glGetUniformLocation(program,"material.k_diffuse"), k_diffuse);
	glUniform1f(glGetUniformLocation(program,"material.k_specular"),k_specular);
	glUniform1f(glGetUniformLocation(program,"material.shininess"), shininess);
	glUniform3f(glGetUniformLocation(App->programShader->defaultProgram, "light.light_dir"), App->modelLoader->light.light_dir.x, App->modelLoader->light.light_dir.y, App->modelLoader->light.light_dir.z);
	glUniform3f(glGetUniformLocation(App->programShader->defaultProgram, "light.color"), App->modelLoader->light.color.x, App->modelLoader->light.color.y, App->modelLoader->light.color.z);

	

}
