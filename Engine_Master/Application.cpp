#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
//#include "ModuleTriangle.h"
#include "ModuleProgramShader.h"
#include "ModuleEditor.h"
#include "ModuleEditorCamera.h"
#include "ModuleTexture.h"
#include "ModuleModelLoader.h"
#include "ModuleTime.h"
#include "MsTimer.h"

using namespace std;

Application::Application()
{
	// Order matters: they will Init/start/update in this order
	modules.push_back(window = new ModuleWindow());
	modules.push_back(time = new ModuleTime());
	modules.push_back(texture = new ModuleTexture());
	modules.push_back(input = new ModuleInput());
	//modules.push_back(triangle = new ModuleTriangle());
	modules.push_back(programShader = new ModuleProgramShader());
	modules.push_back(editorCamera = new ModuleEditorCamera());
	modules.push_back(modelLoader = new ModuleModelLoader());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(renderer = new ModuleRender());

}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	MsTimer timer;
	timer.start();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	timer.stop();
	LOG("init total time: %d ms", timer.read());

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	time->frameStart();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	time->frameEnd();
	
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
