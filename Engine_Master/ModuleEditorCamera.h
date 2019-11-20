#pragma once
#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "Geometry/Frustum.h"
#include "Math/MathAll.h"
#include "SDL.h"


class ModuleEditorCamera : public Module {
public:
	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	float4x4 LookAt(float3 target, float3 eye, float3 up);
	void changeFOV(float fov);
	void ModuleEditorCamera::changePositionX(const float position);
	void ModuleEditorCamera::changePositionY(const float position);
	void ModuleEditorCamera::changePositionZ(const float position);
	void ModuleEditorCamera::changeRotationX(const float rotation);
	void ModuleEditorCamera::changeRotationY(const float rotation);
	void ModuleEditorCamera::changeRotationZ(const float rotation);

public:
	Frustum myFrustum;
	float4x4 proj, view, model;
	float speed;
};

