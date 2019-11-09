#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleRenderExercise;
class ModuleTriangle;
class ModuleProgramShader;
class ModuleEditor;
class ModuleEditorCamera;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleTriangle* triangle = nullptr;
	ModuleProgramShader* programShader = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleEditorCamera* editorCamera = nullptr;

private:
	std::list<Module*> modules;
};

extern Application* App;
