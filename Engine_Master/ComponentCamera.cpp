#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgramShader.h"
#include "ModuleEditorCamera.h"
#include "ModuleTime.h"
#include "GameObject.h"

#include <math.h>
#include "Geometry/Plane.h"
#include "glew.h"
#include "IconsFontAwesome5.h"
#include "DebugDraw.h"
#include "Util/debug_draw.hpp"


ComponentCamera::ComponentCamera()
{
	myType = CAMERA;
	allowMany = false;
	selectedProjectionMode = 0;
	selectedClearMode = 0;

	frustum = new Frustum();
	frustum->type = FrustumType::PerspectiveFrustum;

	SetPlaneDistances(0.1f, 50.0f);
	SetFOV(math::pi / 4.0f);

	frustum->front = -float3::unitZ;
	frustum->up = float3::unitY;

	reloadMatrices();

	clearColor = ImVec4(0.1f, 0.f, 0.02f, 1.f);
}


ComponentCamera::~ComponentCamera()
{
	delete frustum;
}

void ComponentCamera::Update()
{
	frustum->pos = myGameObject->myTransform->position;
	pitch(myGameObject->myTransform->deltaEulerRotation.x, App->time->getDeltaTime());
	yaw(myGameObject->myTransform->deltaEulerRotation.y, App->time->getDeltaTime());
	roll(myGameObject->myTransform->deltaEulerRotation.z, App->time->getDeltaTime());
	
	reloadMatrices();
}

void ComponentCamera::SetFOV(float fov)
{
	frustum->verticalFov = fov;
	frustum->horizontalFov = 3.f * atanf(tanf(fov * 0.5f) * (SCREEN_WIDTH / SCREEN_HEIGHT));
}

void ComponentCamera::SetAspectRatio(float h)
{
	SetFOV(h);
}

void ComponentCamera::SetPlaneDistances(float nearPlane, float farPlane)
{
	frustum->nearPlaneDistance = nearPlane;
	frustum->farPlaneDistance = farPlane;
}

void ComponentCamera::pitch(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction)) * -1.0f;
	float3x3 rotationMatrix = float3x3::RotateAxisAngle(frustum->WorldRight(), adjustment);
	frustum->front = rotationMatrix.Transform(frustum->front).Normalized();
	frustum->up = rotationMatrix.Transform(frustum->up).Normalized();
}

void ComponentCamera::yaw(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction)) * -1.0f;
	float3x3 rotationMatrix = float3x3::RotateY(adjustment);
	frustum->front = rotationMatrix.Transform(frustum->front).Normalized();
	frustum->up = rotationMatrix.Transform(frustum->up).Normalized();
}

void ComponentCamera::roll(float direction, float dt)
{
	float adjustment = CAM_ROTATION_SPEED * (dt * math::DegToRad(direction)) * -1.0f;
	float3x3 rotationMatrix = float3x3::RotateZ(adjustment);
	frustum->front = rotationMatrix.Transform(frustum->front).Normalized();
	frustum->up = rotationMatrix.Transform(frustum->up).Normalized();
}

void ComponentCamera::reloadMatrices()
{
	projectionMatrix = frustum->ProjectionMatrix();
	viewMatrix = frustum->ViewMatrix();
}

int ComponentCamera::AABBWithinFrustum(const AABB &aabb)
{
	FrustumCollisionMode result;

	float3 corners[8];
	aabb.GetCornerPoints(corners);

	int totalWithin = 0;
	for(int p = 0; p < 6; ++p)
	{
		int insideBuffer = 8;
		int pointsWithin = 1;

		for(int i = 0; i< 8; ++i)
		{
			if(sideOfPlane(corners[i], frustum->GetPlane(p)))
			{
				pointsWithin = 0;
				--insideBuffer;
			}
		}
		if (insideBuffer == 0) result = OUTSIDE;
		totalWithin += pointsWithin;
	}

	if (totalWithin == 6) result = INSIDE;
	else result = BETWEEN;

	return result;
}

//True means the point is in front, otherwise it's at the back
bool ComponentCamera::sideOfPlane(float3 &point, Plane &plane)
{
	float value = plane.normal.Dot(point);
	value -= plane.d;

	return (value >= 0.0f) ? true : false;
}

void ComponentCamera::DrawFrustum()
{
	dd::frustum(frustum->ViewProjMatrix().Inverted(), float3(0.f, 0.02f, 0.7f));
}

void ComponentCamera::DrawInspector()
{
	if (ImGui::CollapsingHeader(ICON_FA_CAMERA " Camera"))
	{
		const char* clearModeTags[]{ "Color", "Skybox" };
		ImGui::Combo("Clear mode", &selectedClearMode, clearModeTags, 2);

		if (selectedClearMode == COLOR) ImGui::ColorEdit3("Clear Color", &clearColor.x);

		const char* projectionModeTags[]{ "Perspective", "Ortographic" };
		ImGui::Combo("Projection", &selectedProjectionMode, projectionModeTags, 2);

		if (selectedProjectionMode == PERSPECTIVE)
		{
			frustum->type = FrustumType::PerspectiveFrustum;
			//SetAspectRatio(1, 1);

		}
		else if (selectedProjectionMode == ORTHOGRAPHIC)
		{
			frustum->type = FrustumType::OrthographicFrustum;
		}
		ImGui::Separator();
	}
}