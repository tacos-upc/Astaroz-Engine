#ifndef __ComponentCamera_H__
#define __ComponentCamera_H__

#include "Globals.h"
#include "Component.h"
#include "Geometry/Frustum.h"
#include "Math/float4x4.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "AABBTree.h"
#include <vector>


enum FrustumCollisionMode { OUTSIDE, INSIDE, BETWEEN };

const bool FRONT = true;
const bool BEHIND = false;

enum ClearMode { COLOR, SKYBOX };
enum ProjectionMode { PERSPECTIVE, ORTHOGRAPHIC };

class ComponentCamera : public Component
{
public:
	ComponentCamera();
	ComponentCamera(GameObject* gameObject, ComponentCamera* componentCamera);
	~ComponentCamera();

	void Update() override;

	void SetFOV(float);
	void SetAspectRatio(float);
	void SetPlaneDistances(float, float);
	bool sideOfPlane(float3 &point, Plane &plane);
	void DrawFrustum(float3);
	void DrawInspector() override;

	int AABBWithinFrustum(const AABB &aabb);
	void reloadMatrices();

	void pitch(float, float);
	void yaw(float, float);
	void roll(float, float);

	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);


	Frustum* frustum;
	float4x4 projectionMatrix = float4x4::zero;
	float4x4 viewMatrix = float4x4::zero;
	ImVec4 clearColor;
	int selectedClearMode;

	LineSegment raycast(float3);
	void drawRaycast(LineSegment*);
	void transformFrustum();

	GameObject* getTouchedGameObject(AABBTreeNode*, LineSegment*);

private:
	int selectedProjectionMode;
	float fov;

	std::vector<GameObject*> touchedCandidates;
	void findTouchedCandidates(AABBTreeNode*, LineSegment*);

};

#endif __ComponentCamera_H__