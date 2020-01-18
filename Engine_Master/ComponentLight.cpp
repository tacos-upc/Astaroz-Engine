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
