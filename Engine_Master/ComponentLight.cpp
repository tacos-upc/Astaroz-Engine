#include "ComponentLight.h"
#include "IconsFontAwesome5.h"

ComponentLight::ComponentLight()
{
	myType = LIGHT;
	allowMany = false;
	light = Light();
	light.light_dir = math::float3(1.0f, 1.0f, 1.0f);
	light.color = math::float3(1.0f, 1.0f, 1.0f);
	App->modelLoader->light = light;
}

ComponentLight::~ComponentLight()
{
}

void ComponentLight::DrawInspector()
{
	if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "Light"))
	{
		ImGui::SliderFloat3("light_direction", (float*)&App->modelLoader->light.light_dir, -15.0f, 15.0f);
		ImGui::SliderFloat3("light color", (float*)&App->modelLoader->light.color, -15.0f, 15.0f);
	}
}

void ComponentLight::OnSave(Serialization& serial)
{
	serial.AddInt("Type", myType);
	serial.AddBool("Enabled", isEnabled);
	serial.AddFloat3("Light Direction", light.light_dir);
	serial.AddFloat3("Light Color", light.color);
}

void ComponentLight::OnLoad(const Serialization& serial)
{
	isEnabled = serial.GetBool("Enabled", true);
	serial.GetFloat3("Light Direction", light.light_dir, math::float3(1.0f, 1.0f, 1.0f));
	serial.GetFloat3("Light Color", light.color, math::float3(1.0f, 1.0f, 1.0f));
}