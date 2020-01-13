#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "glew.h"


ModuleRender::ModuleRender()
{
}

// Destructor
ModuleRender::~ModuleRender()
{
}

static void APIENTRY openglCallbackFunction(

	GLenum source,

	GLenum type,

	GLuint id,

	GLenum severity,

	GLsizei length,

	const GLchar* message,

	const void* userParam

) {

	(void)source; (void)type; (void)id;

	(void)severity; (void)length; (void)userParam;



	char error_source[256];

	switch (source)

	{

	case GL_DEBUG_SOURCE_API:             sprintf_s(error_source, "Source: API"); break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sprintf_s(error_source, "Source: Window System"); break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER: sprintf_s(error_source, "Source: Shader Compiler"); break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:     sprintf_s(error_source, "Source: Third Party"); break;

	case GL_DEBUG_SOURCE_APPLICATION:     sprintf_s(error_source, "Source: Application"); break;

	case GL_DEBUG_SOURCE_OTHER:           sprintf_s(error_source, "Source: Other"); break;

	}



	char error_type[256];

	switch (type)

	{

	case GL_DEBUG_TYPE_ERROR:               sprintf_s(error_type, "Type: Error"); break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: sprintf_s(error_type, "Type: Deprecated Behaviour"); break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  sprintf_s(error_type, "Type: Undefined Behaviour"); break;

	case GL_DEBUG_TYPE_PORTABILITY:         sprintf_s(error_type, "Type: Portability"); break;

	case GL_DEBUG_TYPE_PERFORMANCE:         sprintf_s(error_type, "Type: Performance"); break;

	case GL_DEBUG_TYPE_MARKER:              sprintf_s(error_type, "Type: Marker"); break;

	case GL_DEBUG_TYPE_PUSH_GROUP:          sprintf_s(error_type, "Type: Push Group"); break;

	case GL_DEBUG_TYPE_POP_GROUP:           sprintf_s(error_type, "Type: Pop Group"); break;

	case GL_DEBUG_TYPE_OTHER:               sprintf_s(error_type, "Type: Other"); break;

	}



	char error_message[4096];

	sprintf_s(error_message, "%s %s %s", error_source, error_type, message);

	switch (severity)

	{

	case GL_DEBUG_SEVERITY_HIGH:

		OPENGL_LOG_ERROR(error_message);

		break;

	case GL_DEBUG_SEVERITY_MEDIUM:

		OPENGL_LOG_INIT(error_message); 

		break;

	case GL_DEBUG_SEVERITY_LOW:

		//OPENGL_LOG_INFO(error_message); Too many messages in update

	case GL_DEBUG_SEVERITY_NOTIFICATION:

		return;

	}

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

	//Comment CULL_FACE to enable or disable, respectively
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, 1024, 768);


	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(openglCallbackFunction, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

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