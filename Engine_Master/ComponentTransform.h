#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Globals.h"
#include "Component.h"

#include "Math/float3.h"
#include "Math/Quat.h"
#include "Math/float4x4.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameObject);
	~ComponentTransform();

	//public methods
	void EulerToQuat();
	void QuatToEuler();
	void UpdateMatrices();
	void SetGlobalMatrix(float4x4& parentGlobal);
	void SetLocalMatrix(float4x4& newParentGlobalMatrix);

	//public variables
	float3 position = float3(0.0f, 0.0f, 0.0f);
	Quat rotation = Quat::identity;
	float3 eulerRotation = float3(0.0f, 0.0f, 0.0f);
	float3 scale = float3(1.0f, 1.0f, 1.0f);
	float4x4 localModelMatrix = float4x4::identity;
	float4x4 globalModelMatrix = float4x4::identity;
};

#endif __ComponentTransform_H__