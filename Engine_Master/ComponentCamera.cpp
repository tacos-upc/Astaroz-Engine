#include "ComponentCamera.h"
#include "ComponentTransform.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleProgramShader.h"
#include "ModuleEditorCamera.h"
#include "GameObject.h"
#include <math.h>
#include "Geometry/Plane.h"
#include "glew.h"
#include "IconsFontAwesome5.h"


ComponentCamera::ComponentCamera()
{
	myType = CAMERA;

	frustum = new Frustum();
	frustum->type = FrustumType::PerspectiveFrustum;

	SetPlaneDistances(0.1f, 2000.0f);
	SetFOV(math::pi / 4.0f);

	reloadMatrices();

	clearColor = ImVec4(0.36f, 0.9f, 0.39, 1.f);
}


ComponentCamera::~ComponentCamera()
{
	delete frustum;
}

void ComponentCamera::Update()
{
	frustum->pos = myGameObject->myTransform->position;
	viewMatrix = frustum->ViewMatrix();

	return;
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
	//Tests if an AABB is within the frusum
	//returns 0 if out, 1 if in and 2 if intersects
	float3 corners[8];
	aabb.GetCornerPoints(corners);

	int iTotalIn = 0;

	//test all 8 corners against the 6 planes of the frustum
	// if all points are behind 1 specific plane, we are out
	// if we are in with all points, then we are fully in
	for(int p = 0; p < 6; ++p)
	{
		int iInCount = 8;
		int iPtIn = 1;

		for(int i = 0; i< 8; ++i)
		{
			//test this point against the planes
			if(SideOfPlane(corners[i], frustum->GetPlane(p)) == FRONT)
			{
				iPtIn = 0;
				--iInCount;
			}
		}
		
		// were all the points outside of plane p?
		if (iInCount == 0)
			return AABB_OUT;

		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;

	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(AABB_IN);

	return AABB_INTERSECT;
}

bool ComponentCamera::SideOfPlane(float3 &point, Plane &plane)
{
	float value = plane.normal.Dot(point);
	value -= plane.d;

	return (value >= 0.0f) ? true : false;
}

void ComponentCamera::DrawFrustum()
{
	//First we will get the width and height of the near plane
	Hnear = 2 * tan(frustum->verticalFov / 2) * frustum->nearPlaneDistance;
	//Wnear = Hnear * aspect;

	//Then we do the same for the far plane
	Hfar = 2 * tan(frustum->verticalFov / 2) * frustum->farPlaneDistance;
	//Wfar = Hfar * aspect;

	//Now we get the center of the planes
	centerNear = frustum->pos + frustum->front * frustum->nearPlaneDistance;
	centerFar = frustum->pos + frustum->front * frustum->farPlaneDistance;

	//And now we get our points
	NearTopLeft = centerNear + (frustum->up * (Hnear / 2)) - (frustum->WorldRight() * (Wnear / 2));
	NearTopRight = centerNear + (frustum->up * (Hnear / 2)) + (frustum->WorldRight() * (Wnear / 2));
	NearBottomLeft = centerNear - (frustum->up * (Hnear / 2)) - (frustum->WorldRight() * (Wnear / 2));
	NearBottomRight = centerNear - (frustum->up * (Hnear / 2)) + (frustum->WorldRight() * (Wnear / 2));

	FarTopLeft = centerFar + (frustum->up * (Hfar / 2)) - (frustum->WorldRight() * (Wfar / 2));
	FarTopRight = centerFar + (frustum->up * (Hfar / 2)) + (frustum->WorldRight() * (Wfar / 2));
	FarBottomLeft = centerFar - (frustum->up * (Hfar / 2)) - (frustum->WorldRight() * (Wfar / 2));
	FarBottomRight = centerFar - (frustum->up * (Hfar / 2)) + (frustum->WorldRight() * (Wfar / 2));

	GLuint gridProgram = App->programShader->gridProgram;
	glUseProgram(gridProgram);

	//Draw Lines
	glLineWidth(2.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	
	//Near Plane
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);
	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);

	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);
	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);

	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);
	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);

	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);

	//Far Plane
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);
	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);

	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);
	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);

	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);
	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);

	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);

	//Perpendicular planes
	glVertex3f(NearTopLeft.x, NearTopLeft.y, NearTopLeft.z);
	glVertex3f(FarTopLeft.x, FarTopLeft.y, FarTopLeft.z);

	glVertex3f(NearTopRight.x, NearTopRight.y, NearTopRight.z);
	glVertex3f(FarTopRight.x, FarTopRight.y, FarTopRight.z);

	glVertex3f(NearBottomLeft.x, NearBottomLeft.y, NearBottomLeft.z);
	glVertex3f(FarBottomLeft.x, FarBottomLeft.y, FarBottomLeft.z);

	glVertex3f(NearBottomRight.x, NearBottomRight.y, NearBottomRight.z);
	glVertex3f(FarBottomRight.x, FarBottomRight.y, FarBottomRight.z);


	glEnd();

	glUseProgram(0);

	return;
}

void ComponentCamera::drawInspector()
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
