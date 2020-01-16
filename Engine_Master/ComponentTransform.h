#ifndef __ComponentTransform_H__
#define __ComponentTransform_H__

#include "Globals.h"
#include "Component.h"

#include "Math/float3.h"
#include "Math/Quat.h"
#include "Math/float4x4.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"

class ComponentTransform : public Component
{
public:
	ComponentTransform(GameObject* gameObject);
	ComponentTransform(GameObject* gameObject, ComponentTransform* componentTransform);
	~ComponentTransform();

	//public methods
	void EulerToQuat();
	void QuatToEuler();
	void UpdateMatrices();
	void SetGlobalMatrix(float4x4& parentGlobal);
	void SetLocalMatrix(float4x4& newParentGlobalMatrix);
	void DrawInspector() override;
	void OnSave(Serialization& serial);
	void OnLoad(const Serialization& serial);

	//public variables
	float3 position = float3(0.0f, 0.0f, 0.0f);
	Quat rotation = Quat::identity;
	float3 eulerRotation = float3(0.0f, 0.0f, 0.0f);
	float3 deltaEulerRotation = float3(0.f, 0.f, 0.f);

	float3 scale = float3(1.0f, 1.0f, 1.0f);
	float4x4 localModelMatrix = float4x4::identity;
	float4x4 globalModelMatrix = float4x4::identity;
};

#endif __ComponentTransform_H__