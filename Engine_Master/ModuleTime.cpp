#include "ModuleTime.h"
#include "Math/MathAll.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"



ModuleTime::ModuleTime()
{
}


ModuleTime::~ModuleTime()
{
}

bool ModuleTime::Init()
{
	LOG("Init SDL Time");
	bool ret = true;

	if (SDL_Init(SDL_INIT_TIMER) < 0)
	{
		LOG("SDL_TIMER could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		timeScale = 1.0f;
		realTimeBeginTimeStamp = 0.0f;
		realTimeEndTimeStamp = 0.0f;

		timeSinceStartUp = 0.0f;
		framesSinceStartUp = 0.0f;

		timeSinceGameStart = 0.0f;
		framesSinceGameStart = 0.0f;
		state = STOP;
	}

	return ret;
}

update_status ModuleTime::Update()
{
	//ImGui::BeginMenuBar();
	//if (ImGui::BeginMenu("Play"))
	//{
	//	ImGui::EndMenu();
	//};
	//if (ImGui::BeginMenu("Pause"))
	//{
	//	ImGui::EndMenu();
	//};
	//if (ImGui::BeginMenu("Stop"))
	//{
	//	ImGui::EndMenu();
	//};
	//ImGui::EndMenuBar();

	return UPDATE_CONTINUE;
}

void ModuleTime::play()
{
	if (state == PLAY) return;
	state = PLAY;
}

void ModuleTime::tick()
{
	if (state != PAUSE) return;
}

void ModuleTime::pause()
{
	if (state != PLAY) return;
	state = PAUSE;
}

void ModuleTime::stop()
{
	if (state == STOP) return;
	state = STOP;
}

float ModuleTime::realDeltaTime()
{
	return math::Abs(realTimeBeginTimeStamp - realTimeEndTimeStamp) / 1000.0f;
}

float ModuleTime::gameDeltaTime()
{
	return (math::Abs(gameTimeBeginTimeStamp - gameTimeEndTimeStamp) / 1000.0f) * timeScale;
}

float ModuleTime::realTimeSinceStartUp()
{
	return 0.0f;
}

void ModuleTime::setTimeScale(float scale)
{
	if (scale < 0.0f) return;
	timeScale = scale;
}

void ModuleTime::frameStart()
{
	realTimeBeginTimeStamp = SDL_GetTicks();
	if (state == PLAY) gameTimeBeginTimeStamp = SDL_GetTicks();

	//draw();
}

void ModuleTime::frameEnd()
{
	realTimeEndTimeStamp = SDL_GetTicks();
	if (state == PLAY) gameTimeEndTimeStamp = SDL_GetTicks();

	framesSinceStartUp = SDL_GetTicks();
	timeSinceStartUp = framesSinceStartUp / 1000.0f;

	if (state == PLAY)
	{
		framesSinceGameStart = SDL_GetTicks();
		timeSinceGameStart = framesSinceGameStart / 1000.0f;
	}

}

bool ModuleTime::Cleanup()
{
	return true;
}
