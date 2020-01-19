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
	serial.SaveInt("Type", myType);
	serial.SaveBool("Enabled", isEnabled);
	serial.SaveFloat3("Light Direction", App->modelLoader->light.light_dir);
	serial.SaveFloat3("Light Color", App->modelLoader->light.color);
}

void ComponentLight::OnLoad(const Serialization& serial)
{
	isEnabled = serial.LoadBool("Enabled", true);
	light.light_dir = serial.LoadFloat3("Light Direction", math::float3(1.0f, 1.0f, 1.0f));
	light.color = serial.LoadFloat3("Light Color", math::float3(1.0f, 1.0f, 1.0f));
	App->modelLoader->light = light;
}