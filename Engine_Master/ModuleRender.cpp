#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "glew.h";

ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

// Called before render is available
bool ModuleRender::Init()
{
	LOG("Creating Renderer context");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    glcontext = SDL_GL_CreateContext(App->window->window);

	GLenum err = glewInit();
	// … check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));


	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, 1024, 768);

	App->modelLoader->LoadModel(MODEL_BAKER_PATH);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	//Program (shaders: vertex shader + fragment shader)
	glUseProgram(App->programShader->myProgram);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "model"), 1, GL_TRUE, &App->editorCamera->model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "view"), 1, GL_TRUE, &App->editorCamera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->myProgram, "proj"), 1, GL_TRUE, &App->editorCamera->proj[0][0]);

	//Viewport using window size
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
	//Textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, App->texture->myTexture);
	glUniform1i(glGetUniformLocation(App->programShader->myProgram, "texture0"), 0);

	//Attach window and context
	SDL_GL_MakeCurrent(App->window->window, glcontext);

	//Draw program shader
	App->modelLoader->Draw(App->programShader->myProgram);

	return UPDATE_CONTINUE;
}

update_status ModuleRender::PostUpdate()
{
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRender::CleanUp()
{
	LOG("Destroying renderer");

	//Destroy window is done in ModuleWindow
	SDL_GL_DeleteContext(glcontext);

	return true;
}

void ModuleRender::WindowResized(unsigned width, unsigned height)
{

}

