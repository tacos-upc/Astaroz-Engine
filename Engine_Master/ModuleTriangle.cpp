#include "ModuleTriangle.h"


ModuleTriangle::ModuleTriangle()
{

}

ModuleTriangle::~ModuleTriangle()
{

}

bool ModuleTriangle::Init()
{
	float3 v0 = { -1.0f, -1.0f, 0.0f };
	float3 v1 = { 1.0f, -1.0f, 0.0f };
	float3 v2 = { 0.0f, 1.0f, 0.0f };

	/* 
	*****************DONE IN SHADER NOW*******************
	float4x4 transform = proj * view * float4x4(model);
	float4 tmp;
	tmp = transform * float4(v0.x, v0.y, v0.z, 1.0);
	v0 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);

	tmp = transform * float4(v1.x, v1.y, v1.z, 1.0);
	v1 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);

	tmp = transform * float4(v2.x, v2.y, v2.z, 1.0);
	v2 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);
	*/

	float3 buffer_data[] = {
		v0, v1, v2
	};

	//texture
	float new_buffer_data[] = {
		-1.0f, -1.0f, 0.0f, // v0 pos
		1.0f, -1.0f, 0.0f,  // v1 pos
		0.0f,  1.0f, 0.0f,  // v2 pos

		0.0f, 0.0f,         // v0 texcoord   
		1.0f, 0.0f,         // v1 texcoord
		0.5f, 1.0f          // v2 texcoord
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(new_buffer_data), new_buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}

update_status ModuleTriangle::PreUpdate(){
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update() {
	//Use our ModuleProgramShader's program
	glUseProgram(App->programShader->myProgram);

	//Assign buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0); // attribute 0 --> ID
	glVertexAttribPointer(
		0, // attribute 0
		3, // number of componentes (3 floats)
		GL_FLOAT, // data type
		GL_FALSE, // should be normalized?
		0, // stride
		(void*)0 // array buffer offset
	);
	glEnableVertexAttribArray(1); // attribute 1 --> Texture
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)(3 * 3 * sizeof(float)) // buffer offset
	);

	//Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->myTexture);

	//Uniforms
	glUniform1i(glGetUniformLocation(App->programShader->myProgram, "texture0"), 0);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "model"), 1, GL_TRUE, &App->editorCamera->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "view"), 1, GL_TRUE, &App->editorCamera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "proj"), 1, GL_TRUE, &App->editorCamera->proj[0][0]);

	//Draw
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris

	//Clean buffer
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::PostUpdate(){
	return UPDATE_CONTINUE;
}