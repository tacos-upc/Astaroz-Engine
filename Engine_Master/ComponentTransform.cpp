#include "ComponentTransform.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "IconsFontAwesome5.h"
#include "Math/MathFunc.h"

ComponentTransform::ComponentTransform(GameObject* gameObject)
{
	allowMany = false;
	myGameObject = gameObject;
	myType = TRANSFORM;

	onTransformChanged();
}

ComponentTransform::ComponentTransform(GameObject* gameObject, ComponentTransform* componentTransform)
{
	myGameObject = gameObject;
	myType = TRANSFORM;
	position = componentTransform->position;
	rotation = componentTransform->rotation;
	scale = componentTransform->scale;
}

ComponentTransform::~ComponentTransform()
{}


float4x4 ComponentTransform::getGlobalMatrix()
{
	return globalModelMatrix;
}

float4x4 ComponentTransform::getLocalMatrix()
{
	return localModelMatrix;
}

void ComponentTransform::setGlobalMatrix(float4x4& global)
{
	localModelMatrix = myGameObject->parent == nullptr ? global : myGameObject->parent->myTransform->globalModelMatrix.Inverted() * global;

	float3 newPosition;
	float3 newScale;
	float3x3 newRotation;

	localModelMatrix.Decompose(newPosition, newRotation, newScale);

	position = newPosition;
	rotation = newRotation.ToQuat();
	setupEulerRotation(false, rotation.ToEulerXYZ());
	scale = newScale;

	onTransformChanged();
}

void ComponentTransform::setLocalMatrix(float4x4& local)
{
	localModelMatrix = local.Inverted() *  globalModelMatrix;
	localModelMatrix.Decompose(position, rotation, scale);
}

void ComponentTransform::DrawInspector()
{	
	if (ImGui::CollapsingHeader(ICON_FA_HAND_SCISSORS " Transform"))
	{
		if (ImGui::DragFloat3("Position", &position.x)) 
		{
			onTransformChanged();
		}
		if (ImGui::DragFloat3("Rotation", &eulerRotationInDeg.x, 0.1f, -180.f, 180.f)) 
		{
			setupEulerRotation(true, eulerRotationInDeg);
			onTransformChanged();
		}
		if (ImGui::DragFloat3("Scale", &scale.x, 0.1f, 0.0f, 2.f))
		{
			onTransformChanged();
		}

		ImGui::Separator();
	}
}

void ComponentTransform::onTransformChanged()
{
	rotation = Quat::FromEulerXYZ(eulerRotationInDeg.x, eulerRotationInDeg.y, eulerRotationInDeg.z );
	localModelMatrix = float4x4::FromTRS(position, rotation, scale);
	generateGlobalMatrix();

	//deltaEulerRotation = float3(eulerRotation.x - lastEulerRotation.x, eulerRotation.y - lastEulerRotation.y, eulerRotation.z - lastEulerRotation.z);
	
	for (size_t i = 0; i < myGameObject->childrenVector.size(); i++)
	{
		myGameObject->childrenVector.at(i)->myTransform->onTransformChanged();
	}
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

	onTransformChanged();
}

void ComponentTransform::generateGlobalMatrix()
{
	globalModelMatrix = myGameObject->parent == nullptr ? localModelMatrix : myGameObject->parent->myTransform->getGlobalMatrix() * localModelMatrix;
}

void ComponentTransform::setupEulerRotation(bool isDeg, float3 rot)
{
	if (isDeg)
	{
		eulerRotationInDeg = rot;
		eulerRotationInRad = float3(math::DegToRad(rot.x), math::DegToRad(rot.y), math::DegToRad(rot.z));
	}
	else
	{
		eulerRotationInRad = rot;
		eulerRotationInDeg = float3(math::RadToDeg(rot.x), math::RadToDeg(rot.y), math::RadToDeg(rot.z));
	}
}