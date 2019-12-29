#include "ComponentTransform.h"
#include "Math/MathFunc.h"
#include "GameObject.h"
#include "IconsFontAwesome5.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	UpdateMatrices();
}


ComponentTransform::~ComponentTransform()
{
}

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

void ComponentTransform::SetGlobalMatrix(float4x4 &parentGlobal)
{
	globalModelMatrix = parentGlobal * localModelMatrix;
}

void ComponentTransform::SetLocalMatrix(float4x4 &newParentGlobalMatrix)
{
	localModelMatrix = newParentGlobalMatrix.Inverted() *  globalModelMatrix;
	localModelMatrix.Decompose(position, rotation, scale);
	QuatToEuler();
}

void ComponentTransform::drawInspector()
{
	if (ImGui::CollapsingHeader(ICON_FA_HAND_SCISSORS " Transform"))
	{
		ImGui::InputFloat3("Position", &position.x);
		ImGui::InputFloat3("Rotation", &rotation.x);
		ImGui::InputFloat3("Scale", &scale.x);

		ImGui::Separator();
	}
}


