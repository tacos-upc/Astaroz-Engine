#pragma once
#include "Module.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"


class ModuleEditor : public Module{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
	void processEvent(SDL_Event);

public:
	bool scrollToBottom;
	bool show_demo_window;
	bool show_log_window;
	bool show_about_window;
	ImGuiTextBuffer myBuffer;
};

