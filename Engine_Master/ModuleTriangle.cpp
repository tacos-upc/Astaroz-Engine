#include "ModuleTriangle.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgramShader.h"
#include "Geometry/Frustum.h"



ModuleTriangle::ModuleTriangle(){

}


ModuleTriangle::~ModuleTriangle(){

}

bool ModuleTriangle::Init() {

	float3 v0 = { -1.0f, -1.0f, 0.0f };
	float3 v1 = { 1.0f, -1.0f, 0.0f };
	float3 v2 = { 0.0f, 1.0f, 0.0f };

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspect = w / h;

	//TODO empty triangle module and move frustum to EditorCamera module
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);


	float4x4 proj = frustum.ProjectionMatrix();
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	float4x4 view =	LookAt(float3(0.0f, 1.f, 4.0f),	float3(0.0f, 0.0f, 0.0f), float3(0.0f, 1.0f, 0.0f));
	float4x4 transform = proj * view * float4x4(model);

	float4 tmp;
	tmp = transform * float4(v0.x, v0.y, v0.z, 1.0);
	v0 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);

	tmp = transform * float4(v1.x, v1.y, v1.z, 1.0);
	v1 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);

	tmp = transform * float4(v2.x, v2.y, v2.z, 1.0);
	v2 = float3(tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w);


	float3 buffer_data[] = {
		v0, v1, v2
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
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

float4x4 ModuleTriangle::LookAt(float3 eye, float3 target, float3 up) {
	float4x4 matrix;
	math::float3 f(target - eye);
	f.Normalize();
	math::float3 s(f.Cross(up));
	s.Normalize();
	math::float3 u(s.Cross(f));
	matrix[0][0] = s.x;
	matrix[0][1] = s.y;
	matrix[0][2] = s.z;
	matrix[1][0] = u.x;
	matrix[1][1] = u.y;
	matrix[1][2] = u.z;
	matrix[2][0] = -f.x;
	matrix[2][1] = -f.y;
	matrix[2][2] = -f.z;
	matrix[0][3] = -s.Dot(eye);
	matrix[1][3] = -u.Dot(eye);
	matrix[2][3] = f.Dot(eye);
	matrix[3][0] = 0.0f;
	matrix[3][1] = 0.0f;
	matrix[3][2] = 0.0f;
	matrix[3][3] = 1.0f;

	return matrix;
}