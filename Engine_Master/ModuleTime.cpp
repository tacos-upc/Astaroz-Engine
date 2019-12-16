#include "ModuleTime.h"
#include "Math/MathAll.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "IconsFontAwesome5.h"



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
	ImGui::SetNextWindowSize(ImVec2(App->window->width, 8.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 18.0f));
	if (ImGui::Begin("Time Controls", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetCursorPos(ImVec2((size.x/2) - (24*3), size.y/5));
		if (ImGui::Button(ICON_FA_PLAY, ImVec2(24, 24)))
		{
			//Play();
		}

		ImGui::SameLine();


		if (true)
		{
			if (ImGui::Button("PAUSE", ImVec2(24, 24)))
			{
				//Pause();
			}
		}
		else
		{
			if (ImGui::Button("PAUSE", ImVec2(24, 24)))
			{
				//Pause();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("FORWARD", ImVec2(24, 24)))
		{
			//StepFrame();
		}
	}
	ImGui::End();

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
