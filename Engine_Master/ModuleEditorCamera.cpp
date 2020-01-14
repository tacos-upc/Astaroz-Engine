#include "ModuleEditorCamera.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "ModuleEditor.h"
#include "ModuleTime.h"
#include "ModuleSpacePartition.h"
#include "ComponentCamera.h"
#include "Point.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "Math/MathAll.h"
#include "MathGeoLib/include/Geometry/LineSegment.h"
#include "AABBTree.h"
#include "AABBTreeNode.h"
#include "DebugDraw.h"

ModuleEditorCamera::ModuleEditorCamera()
{
}


ModuleEditorCamera::~ModuleEditorCamera()
{

}

bool ModuleEditorCamera::Init()
{
	cam = new ComponentCamera();
	cam->frustum->type = FrustumType::PerspectiveFrustum;
	setDefaultPosition();

	cam->SetPlaneDistances(0.1f, 2000.0f);
	cam->SetFOV(math::pi / 4.0f);

	cam->reloadMatrices();
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
	updateFocus();
	cam->reloadMatrices();

	cam->DrawFrustum(float3(1.f, 0.f, 0.f));


	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::Update()
{
	if (App->editor->getFocusedWindowData()->name == "Scene")
	{
		raycast();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEditorCamera::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleEditorCamera::CleanUp()
{
	delete cam;
	return true;
}

void ModuleEditorCamera::SetPosition(float x, float y, float z)
{
	cam->frustum->pos = float3(x, y, z);
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
	if (navigationMode != MovementMode::FREE) return;

	fPoint mouseMotion = App->input->GetMouseMotion();
	if (math::Abs(mouseMotion.x) > 10.0f) cam->yaw(mouseMotion.x, dt);
	if (math::Abs(mouseMotion.y) > 5.0f) cam->pitch(mouseMotion.y, dt);
}

void ModuleEditorCamera::updateOrbit(float dt)
{
	if (navigationMode != MovementMode::ORBIT) return;

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

void ModuleEditorCamera::updateFocus()
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
		navigationMode = MovementMode::FREE;
		return;
	}
	else navigationMode = MovementMode::MOVEMENT_MODE_NONE ;
	
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT)) navigationMode = MovementMode::ORBIT;
	else navigationMode = MovementMode::MOVEMENT_MODE_NONE;
	
}

void ModuleEditorCamera::LookAt(float3 target)
{
	float3 dir = (target - cam->frustum->pos).Normalized();
	float3x3 rot = float3x3::LookAt(cam->frustum->front, dir, cam->frustum->up, float3::unitY);
	cam->frustum->front = rot.Transform(cam->frustum->front).Normalized();
	cam->frustum->up = rot.Transform(cam->frustum->up).Normalized();
}

float ModuleEditorCamera::getCamSpeed()
{
	return isFastMode ? CAM_SPEED * 2.0f : CAM_SPEED;
}

void ModuleEditorCamera::moveUp(float dt)
{
	float3 newPosition = cam->frustum->pos;
	newPosition.y = newPosition.y + (dt * getCamSpeed());
	cam->frustum->pos = newPosition;
}

void ModuleEditorCamera::moveDown(float dt)
{
	float3 newPosition = cam->frustum->pos;
	newPosition.y = newPosition.y - (dt * getCamSpeed());
	cam->frustum->pos = newPosition;
}

void ModuleEditorCamera::moveLeft(float dt)
{
	cam->frustum->pos -= cam->frustum->WorldRight().ScaledToLength(dt * getCamSpeed());
}

void ModuleEditorCamera::moveRight(float dt)
{
	cam->frustum->pos += cam->frustum->WorldRight().ScaledToLength(dt * getCamSpeed());
}

void ModuleEditorCamera::moveForward(float dt, float extraSpeed)
{
	cam->frustum->pos += cam->frustum->front.ScaledToLength(dt * getCamSpeed() * ((extraSpeed > 0) ? math::Abs(extraSpeed) : 1.0f));
}

void ModuleEditorCamera::moveBackwards(float dt, float extraSpeed)
{
	cam->frustum->pos -= cam->frustum->front.ScaledToLength(dt * getCamSpeed() * ((extraSpeed < 0) ? math::Abs(extraSpeed) : 1.0f));
}

void ModuleEditorCamera::orbitX(float angle, float3 target)
{
	float2 polar = cartesianToPolar(float2(cam->frustum->pos.x, cam->frustum->pos.z), float2(target.x, target.z));
	if (polar.y >= 360.0f) polar.y = 0.0f;
	else if (polar.y <= 0.0f) polar.y = 360.0f;
	polar.y += angle;
	polar.y = math::DegToRad(polar.y);

	float2 newPos = polarToCartesian(polar); 
	cam->frustum->pos.x = newPos.x;
	cam->frustum->pos.z = newPos.y;

	LookAt(target);

}

void ModuleEditorCamera::orbitY(float angle, float3 target)
{
	float2 polar = cartesianToPolar(float2(cam->frustum->pos.z, cam->frustum->pos.y), float2(target.z, target.y));
	if (polar.y >= 360.0f) polar.y = 0.0f;
	else if (polar.y <= 0.0f) polar.y = 360.0f;
	polar.y += angle;
	polar.y = math::DegToRad(polar.y);

	float2 newPos = polarToCartesian(polar);
	cam->frustum->pos.z = newPos.x;
	cam->frustum->pos.y = newPos.y;

	LookAt(target);
}

void ModuleEditorCamera::setDefaultPosition()
{
	cam->frustum->pos = float3(0, 5.0f, 10.0f);
	cam->frustum->front = -float3::unitZ;
	cam->frustum->up = float3::unitY;
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

void ModuleEditorCamera::raycast()
{
	float3 mousePosition = getMouseToViewportPosition();
	if (App->spacePartition->tree->root != nullptr)
	{
		LineSegment segment = cam->raycast(mousePosition);
		bool hitsRoot = segment.Intersects(*App->spacePartition->tree->root->box);
		
		GameObject* touched = cam->getTouchedGameObject(App->spacePartition->tree->root, &segment);

		if(touched != nullptr){}
	}
}

float3 ModuleEditorCamera::getMouseToViewportPosition()
{
	iPoint touch = App->input->GetMousePosition();
	fPoint windowPos = fPoint(App->editor->getFocusedWindowData()->posX, App->editor->getFocusedWindowData()->posY);
	fPoint windowSize = fPoint(App->editor->getFocusedWindowData()->width, App->editor->getFocusedWindowData()->height);

	//Normalize point
	float x = (touch.x - windowPos.x / 2 - windowSize.x / 4) / (windowSize.x / 4);
	float y = -(touch.y - windowPos.y / 2 - windowSize.y / 4) / (windowSize.y / 4);
	float z = 1.0f;

	return float3(x, y, z);
}
