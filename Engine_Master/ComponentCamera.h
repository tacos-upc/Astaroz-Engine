#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"

const int AABB_OUT = 0;
const int AABB_IN = 1;
const int AABB_INTERSECT = 2;

const bool FRONT = true;
const bool BEHIND = false;

enum ClearMode { COLOR, SKYBOX };
enum ProjectionMode { PERSPECTIVE, ORTHOGRAPHIC };

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	void Update() override;

	void SetFOV(float);
	void SetAspectRatio(float);
	void SetPlaneDistances(float, float);
	bool SideOfPlane(float3 &point, Plane &plane);
	void DrawFrustum();
	void drawInspector() override;

	int AABBWithinFrustum(const AABB &aabb);
	void reloadMatrices();

	void pitch(float, float);
	void yaw(float, float);
	void roll(float, float);

	Frustum* frustum;
	float4x4 projectionMatrix = float4x4::zero;
	float4x4 viewMatrix = float4x4::zero;
	ImVec4 clearColor;
	int selectedClearMode = 0;

private:
	//Planes and points
	float Hnear;
	float Wnear;
	float Hfar;
	float Wfar;
	float3 centerNear;
	float3 centerFar;
	float3 NearTopLeft;
	float3 NearTopRight;
	float3 NearBottomLeft;
	float3 NearBottomRight;
	float3 FarTopLeft;
	float3 FarTopRight;
	float3 FarBottomLeft;
	float3 FarBottomRight;

	int selectedProjectionMode = 0;
};

#endif __ComponentCamera_H__
