#include "ModuleEditorCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleTime.h"
#include "Point.h"


ModuleEditorCamera::ModuleEditorCamera()
{
}


ModuleEditorCamera::~ModuleEditorCamera()
{
}

bool ModuleEditorCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	setDefaultPosition();

	SetPlaneDistances(0.1f, 2000.0f);
	SetFOV(math::pi / 4.0f);

	reloadMatrices();
	return true;
}

bool ModuleEditorCamera::Start()
{
	return true;
}

update_status ModuleEditorCamera::PreUpdate()
{
	float dt = App->time->getDeltaTime();

	updateNavModes();
	updatePosition(dt);
	updateRotation(dt);
	updateOrbit(dt);
	updateFocus(dt);

	reloadMatrices();

	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	return true;
}

void ModuleEditorCamera::SetFOV(float fov)
{
	frustum.verticalFov = fov;
	frustum.horizontalFov = 3.f * atanf(tanf(fov * 0.5f) * (SCREEN_WIDTH / SCREEN_HEIGHT));
}

void ModuleEditorCamera::SetAspectRatio(float h)
{
	frustum.horizontalFov = h;
}

void ModuleEditorCamera::SetPlaneDistances(float nearPlane, float farPlane)
{
	frustum.nearPlaneDistance = nearPlane;
	frustum.farPlaneDistance = farPlane;
}

void ModuleEditorCamera::SetPosition(float x, float y, float z)
{
	frustum.pos = float3(x, y, z);
}

void ModuleEditorCamera::updatePosition(float dt)
{
	if (App->input->getWheelSpeed() > 0.1f) moveForward(dt, App->input->getWheelSpeed());
	if (App->input->getWheelSpeed() < -0.1f) moveBackwards(dt, App->input->getWheelSpeed());


	if (!navigationMode == FREE) return;

	if (App->input->isKeyDown(SDL_SCANCODE_E)) moveUp(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_Q)) moveDown(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_A)) moveLeft(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_D)) moveRight(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_W) || App->input->getWheelSpeed() > 0.1f) moveForward(dt, App->input->getWheelSpeed());

	if (App->input->isKeyDown(SDL_SCANCODE_S) || App->input->getWheelSpeed() < -0.1f) moveBackwards(dt, App->input->getWheelSpeed());
}

void ModuleEditorCamera::updateRotation(float dt)
{
	if (navigationMode != FREE) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (math::Abs(mouseMotion.x) > 10.0f) yaw(mouseMotion.x, dt);
	if (math::Abs(mouseMotion.y) > 5.0f) pitch(mouseMotion.y, dt);
}

void ModuleEditorCamera::updateOrbit(float dt)
{
	if (navigationMode != ORBIT) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (math::Abs(mouseMotion.x) > 2.0f && App->input->isKeyDown(SDL_SCANCODE_LALT))
	{
		orbitAngleX += mouseMotion.x;
		orbitX(orbitAngleX, float3(0, 0, 0));
	}

	//I don't think we're doing anyone any favor by orbitting in Y
	//if (math::Abs(mouseMotion.y) > 2.0f)
	//{
	//	orbitAngleY += mouseMotion.y;
	//	orbitY(orbitAngleY, float3(0, 0, 0));
	//}
}

void ModuleEditorCamera::updateFocus(float dt)
{
	if (App->input->isKeyDown(SDL_SCANCODE_F))
	{
		setDefaultPosition();
		LookAt(float3::zero);
	}
}

void ModuleEditorCamera::updateNavModes()
{
	isFastMode = App->input->isKeyDown(SDL_SCANCODE_LSHIFT);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT))
	{
		navigationMode = FREE;
		return;
	}
	else navigationMode = NONE;
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) navigationMode = ORBIT;
	else navigationMode = NONE;
	
}

void ModuleEditorCamera::LookAt(float3 target)
{
	float3 dir = (target - frustum.pos).Normalized();
	float3x3 rot = float3x3::LookAt(frustum.front, dir, frustum.up, float3::unitY);
	frustum.front = rot.Transform(frustum.front).Normalized();
	frustum.up = rot.Transform(frustum.up).Normalized();
}

void ModuleEditorCamera::reloadMatrices()
{
	projectionMatrix = frustum.ProjectionMatrix();
	viewMatrix = frustum.ViewMatrix();
}

float ModuleEditorCamera::getCamSpeed()
{
	return isFastMode ? CAM_SPEED * 2.0f : CAM_SPEED;
}

void ModuleEditorCamera::moveUp(float dt)
{
	float3 newPosition = frustum.pos;
	newPosition.y = newPosition.y + ((0.05f + dt) * getCamSpeed());
	frustum.pos = newPosition;
}

void ModuleEditorCamera::moveDown(float dt)
{
	float3 newPosition = frustum.pos;
	newPosition.y = newPosition.y - ((0.05f + dt) * getCamSpeed());
	frustum.pos = newPosition;
}

void ModuleEditorCamera::moveLeft(float dt)
{
	frustum.pos -= frustum.WorldRight().ScaledToLength((0.05f + dt) * getCamSpeed());
}

void ModuleEditorCamera::moveRight(float dt)
{
	frustum.pos += frustum.WorldRight().ScaledToLength((0.05f + dt) * getCamSpeed());
}

void ModuleEditorCamera::moveForward(float dt, float extraSpeed)
{
	frustum.pos += frustum.front.ScaledToLength((0.05f + dt) * getCamSpeed() * ((extraSpeed > 0) ? math::Abs(extraSpeed) : 1.0f));
}

void ModuleEditorCamera::moveBackwards(float dt, float extraSpeed)
{
	frustum.pos -= frustum.front.ScaledToLength((0.05f + dt) * getCamSpeed() * ((extraSpeed < 0) ? math::Abs(extraSpeed) : 1.0f));
}

void ModuleEditorCamera::pitch(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction)) * -1.0f;
	float3x3 rotationMatrix = float3x3::RotateAxisAngle(frustum.WorldRight(), adjustment);
	frustum.front = rotationMatrix.Transform(frustum.front).Normalized();
	frustum.up = rotationMatrix.Transform(frustum.up).Normalized();
}

void ModuleEditorCamera::yaw(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction)) * -1.0f;
	float3x3 rotationMatrix = float3x3::RotateY(adjustment);
	frustum.front = rotationMatrix.Transform(frustum.front).Normalized();
	frustum.up = rotationMatrix.Transform(frustum.up).Normalized();
}

void ModuleEditorCamera::orbitX(float angle, float3 target)
{
	float2 polar = cartesianToPolar(float2(frustum.pos.x, frustum.pos.z), float2(target.x, target.z));
	if (polar.y >= 360.0f) polar.y = 0.0f;
	else if (polar.y <= 0.0f) polar.y = 360.0f;
	polar.y += angle;
	polar.y = math::DegToRad(polar.y);

	float2 newPos = polarToCartesian(polar); 
	frustum.pos.x = newPos.x;
	frustum.pos.z = newPos.y;

	LookAt(target);

}

void ModuleEditorCamera::orbitY(float angle, float3 target)
{
	float2 polar = cartesianToPolar(float2(frustum.pos.z, frustum.pos.y), float2(target.z, target.y));
	if (polar.y >= 360.0f) polar.y = 0.0f;
	else if (polar.y <= 0.0f) polar.y = 360.0f;
	polar.y += angle;
	polar.y = math::DegToRad(polar.y);

	float2 newPos = polarToCartesian(polar);
	frustum.pos.z = newPos.x;
	frustum.pos.y = newPos.y;

	LookAt(target);
}

void ModuleEditorCamera::setDefaultPosition()
{
	frustum.pos = float3(0, 5.0f, 10.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
}

//x = distance, y = angle
float2 ModuleEditorCamera::polarToCartesian(float2 polar)
{
	float y = polar.x * math::Cos(polar.y);
	float x = polar.x * math::Sin(polar.y);

	return float2(x, y);
}

float2 ModuleEditorCamera::cartesianToPolar(float2 cartesian, float2 target)
{
	float dX = cartesian.x - target.x;
	float dY = cartesian.y - target.y;

	float r = math::SqrtFast(math::Pow(dX, 2) + math::Pow(dY, 2));

	float theta = 0.0f;

	if (dX == 0.0f && dY == 0.0f) theta = 0.0f;
	else if (dX == 0.0f && dY > 0.0f) theta = 0.0f;
	else if (dX > 0.0f && dY == 0.0f) theta = 90.0f;
	else if (dX < 0.0f && dY == 0.0f) theta = 180.0f;
	else if (dX == 0.0f && dY < 0.0f) theta = 270.0f;
	else theta = math::Atan(dY / dX);
	
	return float2(r, theta);
}
