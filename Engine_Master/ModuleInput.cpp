#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "ModuleTexture.h"
#include "ModuleModelLoader.h"

#include "SDL/include/SDL.h"


ModuleInput::ModuleInput()
{
	keys = new KeyState[MAX_KEYS];
	memset(keys, KEY_IDLE, sizeof(KeyState) * MAX_KEYS);
}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

update_status ModuleInput::PreUpdate()
{
	memset(windowEvents, false, WE_COUNT * sizeof(bool));

	SDL_PumpEvents();
	static SDL_Event event;
	keyboard = SDL_GetKeyboardState(NULL);

	for (int i = 0; i < MAX_KEYS; ++i)
	{
		if (keyboard[i] == 1)
		{
			if (keys[i] == KEY_IDLE)
				keys[i] = KEY_DOWN;
			else
				keys[i] = KEY_REPEAT;
		}
		else
		{
			if (keys[i] == KEY_REPEAT || keys[i] == KEY_DOWN)
				keys[i] = KEY_UP;
			else
				keys[i] = KEY_IDLE;
		}
	}

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			windowEvents[WE_QUIT] = true;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_CLOSE:
				windowEvents[WE_QUIT] = true;
				break;
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				App->window->ResizeWindow(event.window.data1, event.window.data2);
				break;
			}
			break;

		case SDL_MOUSEWHEEL:
			App->editorCamera->zoom(event.wheel.y);
			break;

		case SDL_MOUSEMOTION:
			if (event.motion.state & SDL_BUTTON_RMASK) {
				App->editorCamera->rotate((float)event.motion.xrel / (float)App->window->width, (float)event.motion.yrel / (float)App->window->height);
			}
			else if ((event.motion.state & SDL_BUTTON_LMASK) && isKeyDown(SDL_SCANCODE_LALT)) {
				App->editorCamera->orbit((float)event.motion.xrel / (float)App->window->width, (float)event.motion.yrel / (float)App->window->height);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_RIGHT)
				App->editorCamera->allowMovement = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_RIGHT)
				App->editorCamera->allowMovement = true;
			break;

		case SDL_DROPFILE:
			//Save filepath
			const char* file = event.drop.file;

			//We need to know if it's a model or a texture using the file extension
			std::string ext(file);
			std::size_t lastPoint = ext.find_last_of(".");	
			ext = ext.substr(lastPoint + 1, ext.length());	//from last point in the name we can get the (ext)ension

			//Model
			if (ext == "fbx" || ext == "FBX")
			{
				App->modelLoader->loadNewModel(file);
			}

			//Texture
			if (ext == "png" || ext == "jpg" || ext == "dds")
			{
				Texture texture = App->texture->LoadTexture(file);
				//App->modelLoader->addTexture(texture);
			}

			//Free memory
			SDL_free(event.drop.file);
			break;
		}
	}

	if (keyboard[SDL_SCANCODE_ESCAPE] || getWindowEvent(EventWindow::WE_QUIT) == true)
		return UPDATE_STOP;
	
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleInput::Update()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

bool ModuleInput::getWindowEvent(EventWindow ev) const
{
	return windowEvents[ev];
}

KeyState ModuleInput::getKey(int id) const
{
	return keys[id];
}

bool ModuleInput::isKeyDown(int id) const
{
	return keys[id] == KEY_DOWN || keys[id] == KEY_REPEAT;
}