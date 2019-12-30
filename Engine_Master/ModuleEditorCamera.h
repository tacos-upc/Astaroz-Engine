#ifndef __ModuleEditorCamera_H__
#define __ModuleEditorCamera_H__

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
	void zoom(const float wheelY);
	void changePositionX(const float position);
	void changePositionY(const float position);
	void changePositionZ(const float position);
	void rotate(const float mouseMotionX, const float mouseMotionY);
	void orbit(const float mouseMotionX, const float mouseMotionY);

public:
	Frustum myFrustum;
	float4x4 proj, view, model;
	float zoomSpeed, rotSpeed, multiplierMovSpeed, pitch, yaw, radius, orbitX, orbitY;
	bool allowMovement;
};

#endif __ModuleEditorCamera_H__