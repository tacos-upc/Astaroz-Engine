#include "ModuleEditorCamera.h"
#include "ModuleModelLoader.h"
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
	frustum.pos = float3::unitX;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

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
	updateNavModes();
	updatePosition(0.06f);
	updateRotation(0.06f);
	updateOrbit(0.06f);

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
	if (!navigationMode == FREE) return;

	if (App->input->isKeyDown(SDL_SCANCODE_W)) moveUp(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_S)) moveDown(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_A)) moveLeft(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_D)) moveRight(dt);

	if (App->input->isKeyDown(SDL_SCANCODE_E) || App->input->getWheelSpeed() > 0.0f) moveForward(dt, App->input->getWheelSpeed());

	if (App->input->isKeyDown(SDL_SCANCODE_Q) || App->input->getWheelSpeed() < 0.0f) moveBackwards(dt, App->input->getWheelSpeed());
}

void ModuleEditorCamera::updateRotation(float dt)
{
	if (navigationMode != FREE) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (math::Abs(mouseMotion.x) > 10.0f) yaw(mouseMotion.x, dt);
	if (math::Abs(mouseMotion.y) > 5.0f) pitch(mouseMotion.y, dt);

	//if (App->input->isKeyDown(SDL_SCANCODE_LEFT)) yaw(true, dt);
	//if (App->input->isKeyDown(SDL_SCANCODE_RIGHT)) yaw(false, dt);
	//
	//if (App->input->isKeyDown(SDL_SCANCODE_UP)) pitch(true, dt);
	//if (App->input->isKeyDown(SDL_SCANCODE_DOWN)) pitch(false, dt);
}

void ModuleEditorCamera::updateOrbit(float dt)
{
	if (navigationMode != ORBIT) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (mouseMotion.x > 2.0f) yaw(mouseMotion.x, dt);
	if (mouseMotion.y > 2.0f) pitch(mouseMotion.y, dt);
}

void ModuleEditorCamera::updateFocus(float dt)
{
	//TODO: This thing
}

void ModuleEditorCamera::updateNavModes()
{
	isFastMode = App->input->isKeyDown(SDL_SCANCODE_LSHIFT);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT)) navigationMode = FREE;
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) && App->input->isKeyDown(SDL_SCANCODE_LALT)) navigationMode = ORBIT;
	else navigationMode = NONE;
}

void ModuleEditorCamera::LookAt(float3 target)
{
	float3x3 rotation = float3x3::LookAt(frustum.front, (target - frustum.pos).Normalized(), frustum.up, float3::unitY);
	frustum.front = frustum.front * rotation;
	frustum.up = frustum.up * rotation;
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
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction));
	float3x3 rotationMatrix = float3x3::RotateX(adjustment);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

	//LookAt(float3::zero);
}

void ModuleEditorCamera::yaw(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction));
	float3x3 rotationMatrix = float3x3::RotateY(adjustment);
	frustum.up = rotationMatrix * frustum.up;
	frustum.front = rotationMatrix * frustum.front;

	//LookAt(float3::zero);
}

void ModuleEditorCamera::orbitX(float angle, float dt)
{
	const float newAngle = dt * getCamSpeed() * (angle*-1);
	float3x3 rotation_matrix = float3x3::RotateY(newAngle);
	frustum.pos = rotation_matrix * frustum.pos;

	LookAt(float3::zero);

}

void ModuleEditorCamera::orbitY(float angle, float dt)
{
	float adjustment = dt * getCamSpeed() * (angle*-1);
	float newAngle = math::Abs(adjustment + asinf(frustum.front.y / frustum.front.Length()));
	if (newAngle >= math::pi / 2) return;

	float3x3 rotationMatrix = float3x3::identity;
	rotationMatrix.SetRotatePart(frustum.WorldRight(), adjustment);
	frustum.pos = rotationMatrix * frustum.pos;

	LookAt(float3::zero);
}