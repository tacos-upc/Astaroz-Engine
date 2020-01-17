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

		gameTimeBeginTimeStamp = 0.0f;
		gameTimeEndTimeStamp = 0.0f;

		timeSinceStartUp = 0.0f;
		framesSinceStartUp = 0.0f;

		timeSinceGameStart = 0.0f;
		framesSinceGameStart = 0.0f;

		realDeltaTime = 0.0f;
		gameDeltaTime = 0.0f;

		state = PAUSE;
	}

	return ret;
}

update_status ModuleTime::Update()
{
	drawTimeControls();

	return UPDATE_CONTINUE;
}

void ModuleTime::play()
{
	if (state == PLAY) return;
	state = PLAY;
}

void ModuleTime::pause()
{
	if (state != PLAY) return;
	state = PAUSE;
}

void ModuleTime::tick()
{
	if (state != PAUSE) return;
	state = TICK;
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

float ModuleTime::getDeltaTime()
{
	return state == PLAY? gameDeltaTime : realDeltaTime;
}

float ModuleTime::getGameTimeSinceStartup()
{
	return timeSinceGameStart;
}

void ModuleTime::frameStart()
{
	realTimeBeginTimeStamp = SDL_GetTicks();
	if (state == PLAY || state == TICK)
	{
		gameTimeBeginTimeStamp = SDL_GetTicks();

		if (state == TICK) state = PAUSE;
	}
}

void ModuleTime::frameEnd()
{
	realTimeEndTimeStamp = SDL_GetTicks();

	framesSinceStartUp++;
	timeSinceStartUp = SDL_GetTicks() / 1000.0f;

	realDeltaTime = (realTimeEndTimeStamp - realTimeBeginTimeStamp) / 1000.0f;

	if (state == PLAY)
	{
		if (framesSinceGameStart <= 0)
		{
			state = TICK;
			frameEnd();
			state = PLAY;
			return;
		}
		gameTimeEndTimeStamp = SDL_GetTicks();
		framesSinceGameStart++;
		
		gameDeltaTime = ((gameTimeEndTimeStamp - gameTimeBeginTimeStamp) / 1000.0f) * timeScale;
		
		timeSinceGameStart += gameDeltaTime;
	}
	else if (state == TICK)
	{
		framesSinceGameStart++;
		
		gameDeltaTime = ((realTimeEndTimeStamp - realTimeBeginTimeStamp) / 1000.0f) * timeScale;
		timeSinceGameStart += gameDeltaTime;

		gameDeltaTime = 0.0f;
	}
}

bool ModuleTime::Cleanup()
{
	return true;
}

void ModuleTime::drawTimeControls()
{
	ImGui::SetNextWindowSize(ImVec2(App->window->width, 8.0f));
	ImGui::SetNextWindowPos(ImVec2(0.0f, 18.0f));
	if (ImGui::Begin("Time Controls", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar))
	{
		ImVec2 size = ImGui::GetWindowSize();
		ImGui::SetCursorPos(ImVec2((size.x / 2) - (24 * 3), size.y / 5));

		if (ImGui::Button(ICON_FA_PLAY, ImVec2(24, 24))) play();
		
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_PAUSE, ImVec2(24, 24))) pause();
		
		ImGui::SameLine();
		if (ImGui::Button(ICON_FA_FORWARD, ImVec2(24, 24))) tick();
	}
	ImGui::End();
}

void ModuleTime::drawTimeData()
{	
	ImGui::Text("Game Status: %s", stateToString());

	ImGui::Separator();

	ImGui::Text("Time since Startup: %.2fs", timeSinceStartUp);
	ImGui::Text("Frames since Startup: %.0f frames", framesSinceStartUp);
	ImGui::Text("Real Delta time: %.4fs", realDeltaTime);

	ImGui::Separator();

	ImGui::Text("Game Time since startup: %.2fs", timeSinceGameStart);
	ImGui::Text("Game Frames since startup: %.0f frames", framesSinceGameStart);
	ImGui::Text("Game Delta time: %.4fs", gameDeltaTime);

	ImGui::Separator();

	ImGui::SliderFloat("Time Scale", &timeScale, MIN_TIMESCALE, MAX_TIMESCALE, "%.2f", 1.0f);	
}

const char* ModuleTime::stateToString()
{
	char* stringState = "";
	switch (state)
	{
		case PLAY:
			stringState = "Play";
			break;
		case PAUSE:
			stringState = "Pause";
			break;
		case TICK:
			stringState = "One Tick";
			break;
	}

	return stringState;
}
