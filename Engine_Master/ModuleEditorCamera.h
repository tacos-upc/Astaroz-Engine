#ifndef __ModuleEditorCamera_H__
#define __ModuleEditorCamera_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ComponentCamera.h"
#include "Geometry/Frustum.h"
#include "Math/MathAll.h"
#include "SDL.h"


#define CAM_SPEED 12.5f
#define CAM_ROTATION_SPEED 2.7f //160 deg

enum MovementMode
{
	MOVEMENT_ERROR = -1,
	MOVEMENT_MODE_NONE,
	FREE,
	ORBIT,
	MOVEMENT_MODE_COUNT
};

class ModuleEditorCamera :
	public Module
{
public:
	ComponentCamera* cam;

	ModuleEditorCamera();
	~ModuleEditorCamera();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void SetPosition(float, float, float);
	void updatePosition(float);
	void updateRotation(float);
	void updateOrbit(float);
	void updateFocus();
	void updateNavModes();
	void LookAt(float3);

private:

	MovementMode navigationMode = MOVEMENT_MODE_NONE;
	bool isFastMode = false;
	float orbitAngleX = 0.0f;
	float orbitAngleY = 0.0f;

	float getCamSpeed();

	void moveUp(float);
	void moveDown(float);
	void moveLeft(float);
	void moveRight(float);
	void moveForward(float, float speed = 0.0f);
	void moveBackwards(float, float speed = 0.0f);

	void orbitX(float, float3);
	void orbitY(float, float3);

	void setDefaultPosition();

	float2 polarToCartesian(float2);
	float2 cartesianToPolar(float2, float2);
};

#endif __ModuleEditorCamera_H__