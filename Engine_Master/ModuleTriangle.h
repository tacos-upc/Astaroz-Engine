#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL.h"
#include "Geometry/Frustum.h"
#include "Math/MathAll.h"

class ModuleTriangle :	public Module{
public:
	ModuleTriangle();
	virtual ~ModuleTriangle();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	float4x4 LookAt(float3 target, float3 eye, float3 up);

private:
	GLuint vbo;
	float4x4 proj, view, model;
	Frustum frustum;
};

