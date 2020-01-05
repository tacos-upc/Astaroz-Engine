#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "ModuleScene.h"
#include "Skybox.h"
#include "glew.h"
#include "ModuleDebugDraw.h"
#include "debugdraw.h"

ModuleRender::ModuleRender()
{}

// Destructor
ModuleRender::~ModuleRender()
{}

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
	// � check for errors
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	//Comment CULL_FACE to enable or disable, respectively
	glDisable(GL_CULL_FACE);
	//glEnable(GL_CULL_FACE);

	glEnable(GL_TEXTURE_2D);
	glViewport(0, 0, App->window->width, App->window->height);

	generateBuffers(&sceneFBO, &sceneTexture, &sceneRBO);
	generateBuffers(&gameFBO, &gameTexture, &gameRBO);

	skybox = new Skybox("textures/skybox/sides.png", "textures/skybox/sides.png", "textures/skybox/top.png", "textures/skybox/bottom.png", "textures/skybox/sides.png", "textures/skybox/sides.png");
	
	sceneClearColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	gridColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	//Program (shaders: vertex shader + fragment shader)
	glUseProgram(App->programShader->defaultProgram);
	
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &App->editorCamera->cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &App->editorCamera->cam->projectionMatrix[0][0]);

	//Viewport using window size
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRender::Update()
{
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

	glDeleteFramebuffers(1, &sceneFBO);
	glDeleteTextures(1, &sceneTexture);
	glDeleteRenderbuffers(1, &sceneRBO);

	//Destroy window is done in ModuleWindow
	SDL_GL_DeleteContext(glcontext);

	return true;
}


void ModuleRender::renderGrid(ComponentCamera* cam)
{
	if (!usesGrid) return;

	GLuint gridProgram = App->programShader->gridProgram;
	glUseProgram(gridProgram);

	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "view"), 1, GL_TRUE, &cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "proj"), 1, GL_TRUE, &cam->projectionMatrix[0][0]);

	dd::xzSquareGrid(-100.f, 100.f, 0.f, 5.f, float3(gridColor.x, gridColor.y, gridColor.z));
	dd::axisTriad(float4x4::identity, 0.f, 1.f);

	glUseProgram(App->programShader->defaultProgram);
}

void ModuleRender::generateBuffers(GLuint* fbo, GLuint* texture, GLuint* rbo)
{
	glGenFramebuffers(1, fbo);
	glGenTextures(1, texture);
	glGenRenderbuffers(1, rbo);
}


bool ModuleRender::beginRenderTexture(int width, int height, GLuint* fbo, GLuint* texture, GLuint* rbo)
{
	//Generate the frame buffer and bind it
	glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
	
	//Create the texture to fill with the created framebuffer
	glBindTexture(GL_TEXTURE_2D, *texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *texture, 0);

	//Render buffer for depth testing
	glBindRenderbuffer(GL_RENDERBUFFER, *rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	//Atach the render buffer object to the default render buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rbo);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool ModuleRender::endRenderTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	return true;
}

void ModuleRender::drawAllBoundingBoxes()
{
	GLuint gridProgram = App->programShader->gridProgram;
	glUseProgram(gridProgram);

	App->scene->drawAllBoundingBoxes();

	glUseProgram(0);
}

void ModuleRender::drawSceneView()
{
	ImVec2 size = ImGui::GetWindowSize();

	beginRenderTexture(size.x, size.y, &sceneFBO, &sceneTexture, &sceneRBO);

	glViewport(0, 0, App->window->width, App->window->height);
	glClearColor(sceneClearColor.x, sceneClearColor.y, sceneClearColor.z, sceneClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &App->editorCamera->cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &App->editorCamera->cam->projectionMatrix[0][0]);

	//Todo: Update this thing with mesh gameobjects
	App->modelLoader->DrawAll(App->programShader->defaultProgram);

	//This doesn't seem to be working well //Todo: Update/delete comments
	drawAllBoundingBoxes();

	renderGrid(App->editorCamera->cam);

	ImGui::GetWindowDrawList()->AddImage(
		(void *)sceneTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x, ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	App->debugDraw->Draw(App->editorCamera->cam, sceneFBO, App->window->width, App->window->height);
	endRenderTexture();
}

void ModuleRender::drawGameView()
{
	ComponentCamera* cam = (ComponentCamera*)App->scene->mainCamera->GetComponent(CAMERA);
	ImVec2 size = ImGui::GetWindowSize();

	beginRenderTexture(size.x, size.y, &gameFBO, &gameTexture, &gameRBO);

	glViewport(0, 0, App->window->width, App->window->height);
	glClearColor(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, cam->clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &cam->projectionMatrix[0][0]);

	drawGameObjects(App->programShader->defaultProgram);


	if(cam->selectedClearMode == SKYBOX) skybox->draw(cam);

	ImGui::GetWindowDrawList()->AddImage(
		(void *)gameTexture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x, ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	endRenderTexture();
}

void ModuleRender::drawSceneRenderSettings()
{
	ImGui::ColorEdit3("Clear Color", &sceneClearColor.x);
	
	ImGui::Separator();

	ImGui::Checkbox("Uses grid?", &usesGrid);
	if (usesGrid) ImGui::ColorEdit3("Grid Color", &gridColor.x);
}

//TODO: Update this method with proper gameobjects
void ModuleRender::drawGameObjects(GLuint program)
{
	for (size_t i = 0; i < App->modelLoader->meshes.size(); i++)
	{
		if (((ComponentCamera*)App->scene->mainCamera->GetComponent(CAMERA))->AABBWithinFrustum(App->modelLoader->myBoundingBox) != OUTSIDE)
		{
			App->modelLoader->meshes[i]->Draw(program);
		}
	}
}