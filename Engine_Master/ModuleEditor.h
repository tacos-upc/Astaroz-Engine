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
#include "ImGUI/ImGuizmo.h"


struct WindowData
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
	bool scrollToBottom;

	ImGuiTextBuffer myBuffer;
	std::vector<float> fps_log;
	bool isFocused(const char*);
	bool isHovered(const char*);
	bool isFocusedAndHovered(const char*);
	WindowData* getFocusedWindowData();
	WindowData* getHoveredWindowData();

private:
	bool show_about_window, show_configuration_window;
	bool openFileDialog;

	void loadIcons();
	void drawMainMenu();
	void drawHierarchyPanel();
	void drawScenePanel();
	void drawGamePanel();
	void drawInspectorPanel();
	void drawComponentsMenu(float);
	void drawLogPanel();
	void drawSubMenu();
	void drawFileBrowser();
	void updateWindowData(const char*);
	void updateFocusedWindowData(const char*);
	void updateHoveredWindowData(const char*);

	bool openComponentsMenu;
	WindowData* focusedWindowData;
	WindowData* hoveredWindowData;
};

#endif __ModuleEditor_H__