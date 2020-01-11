#ifndef __ModuleEditor_H__
#define __ModuleEditor_H__

#include "Module.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleEditorCamera.h"

#include "SDL/include/SDL.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"


struct FocusedWindowData
{
	const char* name;
	float posX;
	float posY;
	float width;
	float height;
};

class ModuleEditor : public Module{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	void processEvent(SDL_Event);

public:
	bool scrollToBottom, show_about_window, show_configuration_window;
	ImGuiTextBuffer myBuffer;
	std::vector<float> fps_log;
	FocusedWindowData* getFocusedWindowData();

private:
	void loadIcons();
	void drawMainMenu();
	void drawHierarchyPanel();
	void drawCameraPanel();
	void drawInspectorPanel();
	void drawComponentsMenu(float);
	void drawLogPanel();
	void updateFocusedWindowData(const char*);

	bool openComponentsMenu;
	FocusedWindowData* focusedWindowData;
};

#endif __ModuleEditor_H__