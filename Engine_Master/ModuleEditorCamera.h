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
	float calculateAspectRatio();
	void setAspectFrustum();
	void focusModel();
	void changePositionX(const float position);
	void changePositionY(const float position);
	void changePositionZ(const float position);
	void changeRotationX(const float rotation);
	void changeRotationY(const float rotation);
	void changeRotationZ(const float rotation);

public:
	Frustum myFrustum;
	float4x4 proj, view, model;
	float speed, pitch, yaw, radius;
};

