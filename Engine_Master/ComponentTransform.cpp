#include "ComponentTransform.h"
#include "GameObject.h"

#include "IconsFontAwesome5.h"
#include "Math/MathFunc.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	allowMany = false;
	myGameObject = gameObject;
	myType = TRANSFORM;
	UpdateMatrices();
}

ComponentTransform::ComponentTransform(GameObject* gameObject, ComponentTransform* componentTransform)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	position = componentTransform->position;
	rotation = componentTransform->rotation;
	scale = componentTransform->scale;
	UpdateMatrices();
}

ComponentTransform::~ComponentTransform()
{}

void ComponentTransform::EulerToQuat()
{
	rotation = rotation.FromEulerXYX(DegToRad(eulerRotation).x, DegToRad(eulerRotation).y, DegToRad(eulerRotation).z);
}

void ComponentTransform::QuatToEuler()
{
	eulerRotation = rotation.ToEulerXYZ();
}

void ComponentTransform::UpdateMatrices()
{
	globalModelMatrix = globalModelMatrix * localModelMatrix.Inverted();
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	globalModelMatrix = globalModelMatrix * localModelMatrix;
}

void ComponentTransform::SetGlobalMatrix(float4x4& parentGlobal)
{
	globalModelMatrix = parentGlobal * localModelMatrix;
}

void ComponentTransform::SetLocalMatrix(float4x4& newParentGlobalMatrix)
{
	localModelMatrix = newParentGlobalMatrix.Inverted() *  globalModelMatrix;
	localModelMatrix.Decompose(position, rotation, scale);
	QuatToEuler();
}

void ComponentTransform::DrawInspector()
{
	float3 lastEulerRotation = float3(eulerRotation.x, eulerRotation.y, eulerRotation.z);
	
	if (ImGui::CollapsingHeader(ICON_FA_HAND_SCISSORS " Transform"))
	{
		ImGui::DragFloat3("Position", &position.x);
		ImGui::DragFloat3("Rotation", &eulerRotation.x);
		ImGui::DragFloat3("Scale", &scale.x);

		ImGui::Separator();
	}

	deltaEulerRotation = float3(eulerRotation.x - lastEulerRotation.x, eulerRotation.y - lastEulerRotation.y, eulerRotation.z - lastEulerRotation.z);
}

void ComponentTransform::OnSave(Serialization& serial)
{
	serial.AddInt("Type", myType);
	serial.AddBool("Enabled", isEnabled);
	serial.AddFloat3("Position", position);
	serial.AddQuat("Rotation", rotation);
	serial.AddFloat3("Scale", scale);
}

void ComponentTransform::OnLoad(const Serialization& serial)
{
	isEnabled = serial.GetBool("Enabled", true);
	serial.GetFloat3("Position", position, float3::zero);
	serial.GetQuat("Rotation", rotation, Quat::identity);
	serial.GetFloat3("Scale", scale, float3::one);

	//TODO: Update matrices
}