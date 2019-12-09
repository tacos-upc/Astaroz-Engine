#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"

const int AABB_OUT = 0;
const int AABB_IN = 1;
const int AABB_INTERSECT = 2;

const bool FRONT = true;
const bool BEHIND = false;

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	~ComponentCamera();

	//Module
	void Update();

	//Member functions
	void SetFOV();
	void SetAspectRatio(int newHeight, int newWidth);
	void Rotate(const float dx, const float dy);
	void Move(float3 direction);
	void TranslateCameraToPoint(const float3 &newPos);
	void SetNearPlaneDistance(const float nearDist);
	void SetFarPlaneDistance(const float farDist);
	void LookAt(const float3 target);
	bool SideOfPlane(float3 &point, Plane &plane);
	void DrawFrustum();

	//Member variables
	int AABBWithinFrustum(const AABB &aabb);
	float aspect = 1.0f;
	float movementSpeed = 0.2f;
	float rotationSpeed = 0.015f;
	float zoomSpeed = 0.5f;
	float motionOffset = 2.5f;
	unsigned int frustumVAO = 0; 
	Frustum* frustum;
	float4x4 proj = float4x4::zero;
	float4x4 view = float4x4::zero;
	float3 oldPosition;

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
};

#endif __ComponentCamera_H__
