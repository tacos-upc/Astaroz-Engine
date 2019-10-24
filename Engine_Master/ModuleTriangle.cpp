#include "ModuleTriangle.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Math/MathAll.h"
#include "Geometry/Frustum.h"



ModuleTriangle::ModuleTriangle(){

}


ModuleTriangle::~ModuleTriangle(){

}

bool ModuleTriangle::Init() {
	float buffer_data[] = {-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f };

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(buffer_data), buffer_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	float aspect = w / h;

	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * aspect);
	//math::float4x4 proj = frustum.ProjectionMatrix();


	float4x4 proj = frustum.ProjectionMatrix();
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	//float4x4 view =	LookAt(view, math::float3(0.0f, 1.f, 4.0f),	math::float3(0.0f, 0.0f, 0.0f), math::float3(0.0f, 1.0f, 0.0f));
	//float4x4 transform = proj * view * float4x4(model);

	return true;
}

update_status ModuleTriangle::PreUpdate(){
	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::Update() {
	glEnableVertexAttribArray(0); // attribute 0
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0, // attribute 0
		3, // number of componentes (3 floats)
		GL_FLOAT, // data type
		GL_FALSE, // should be normalized?
		0, // stride
		(void*)0 // array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3); // start at 0 and 3 tris
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return UPDATE_CONTINUE;
}

update_status ModuleTriangle::PostUpdate(){
	return UPDATE_CONTINUE;
}
