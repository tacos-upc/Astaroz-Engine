#include "Mesh.h"
#include "Application.h"
#include "ModuleProgramShader.h"
#include "ModuleModelLoader.h"
#include "glew.h"
#include <string>

Mesh::Mesh()
{
	
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture*>& textures, unsigned int material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->material = material;

	setupMesh();
}

Mesh::~Mesh()
{
	vertices.clear();
	indices.clear();
	textures.clear();

	//Delete buffers if they exist at this point in the program
	if (VAO != 0)
		glDeleteVertexArrays(1, &VAO);
	if (VBO != 0)
		glDeleteBuffers(1, &VBO);
	if (EBO != 0)
		glDeleteBuffers(1, &EBO);
}

void Mesh::Init()
{
	
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	
	// vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::Draw(unsigned int program) const
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	
	//Assigning "ids" to textures

	//glUniform1i(glGetUniformLocation(App->programShader->myProgram, "material.diffuse_texture"), 0);

	//glUniform1i(glGetUniformLocation(App->programShader->myProgram, "material.specular_texture"), 1);

	//glUniform1i(glGetUniformLocation(App->programShader->myProgram, "material.occlusion_texture"), 2);

	//glUniform1i(glGetUniformLocation(App->programShader->myProgram, "material.emissive_texture"), 3);



	//applying textures to the variables deppending on its id

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[material]->diffuse_texture);

	glUniform1i(glGetUniformLocation(App->programShader->myProgram, "material.diffuse_texture"), 0);

	//glActiveTexture(GL_TEXTURE1);

	//glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[material]->specular_texture);



	//glActiveTexture(GL_TEXTURE2);

	//glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[material]->occlusion_texture);
	


	//glActiveTexture(GL_TEXTURE3);

	//glBindTexture(GL_TEXTURE_2D, App->modelLoader->materials[material]->emissive_texture);
	
	
	
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	glActiveTexture(GL_TEXTURE0);
	//if (hastext) {
	//	for (unsigned int i = 0; i < textures.size(); i++)
	//	{
	//		glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 1);
	//		glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
	//
	//		// retrieve texture number (the N in diffuse_textureN)
	//		std::string number;
	//		std::string name = textures[i].type;
	//		if (name == "texture_diffuse")
	//			number = std::to_string(++diffuseNr);
	//		else if (name == "texture_specular")
	//			number = std::to_string(++specularNr);
	//		else if (name == "texture_normal")
	//			number = std::to_string(++normalNr);
	//		else if (name == "texture_height")
	//			number = std::to_string(++heightNr);
	//		glUniform1i(glGetUniformLocation(program, (name + number).c_str()), i);
	//		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	//	}
	//
	//	//This will actually Draw the prepared mesh
	//	glBindVertexArray(VAO);
	//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	glBindVertexArray(0);
	//	glActiveTexture(GL_TEXTURE0);
	//}
	//else {
	//
	//	glUniform3fv(glGetUniformLocation(program, "light_pos"), 1, (const float*)&App->modelLoader->light_pos);
	//	//glUniform1f(glGetUniformLocation(program, "ambient"), App->modelLoader->ambient);
	//	//glUniform1f(glGetUniformLocation(program, "shininess"), App->modelLoader->materials[material].shininess);
	//	//glUniform1f(glGetUniformLocation(program, "k_ambient"), App->modelLoader->materials[material].k_ambient);
	//	//glUniform1f(glGetUniformLocation(program, "k_diffuse"), App->modelLoader->materials[material].k_diffuse);
	//	//glUniform1f(glGetUniformLocation(program, "k_specular"), App->modelLoader->materials[material].k_specular);
	//	//glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&transform);
	//	App->modelLoader->materials[material]->SetUniforms();
	//	//glUniform1i(glGetUniformLocation(program, "use_diffuse_map"), 0);
	//	//glUniform4fv(glGetUniformLocation(program, "object_color"), 1, (const float*)&App->modelLoader->materials[material].object_color);
	//	glBindVertexArray(VAO);
	//	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
	//	glBindVertexArray(0);
	//	
	//
	//}


	
}

void Mesh::updateTexture(Texture* texture)
{
	for (auto &lastTexture : textures)
	{
		lastTexture = texture;
	}
}

void Mesh::setUniforms()
{

	App->modelLoader->materials[material]->SetUniforms();
	
}

