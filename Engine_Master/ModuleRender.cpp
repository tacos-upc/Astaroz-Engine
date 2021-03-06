#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "ModuleSpacePartition.h"
#include "ModuleScene.h"
#include "Skybox.h"
#include "glew.h"
#include "ModuleDebugDraw.h"
#include "debugdraw.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"


ModuleRender::ModuleRender()
{}

// Destructor
ModuleRender::~ModuleRender()
{}

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
	AABBColor = ImVec4(1.f, 1.f, 1.f, 1.0f);


	glEnable(GL_DEBUG_OUTPUT);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(openglCallbackFunction, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);

	return true;
}

update_status ModuleRender::PreUpdate()
{
	//Program (shaders: vertex shader + fragment shader)

	glUseProgram(App->programShader->defaultProgram);

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
	
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void ModuleRender::drawAllBoundingBoxes()
{
	GLuint gridProgram = App->programShader->gridProgram;
	glUseProgram(gridProgram);

	App->scene->drawAllBoundingBoxes();
	if (renderAABBTree) App->spacePartition->drawTree(AABBColor);

	glUseProgram(0);
}

void ModuleRender::drawGizmos(float posX, float posY, float width, float height)
{
	ImGuizmo::SetRect(posX, posY, width, height);
	ImGuizmo::SetDrawlist();

	if (App->scene->selectedByHierarchy != nullptr)
	{
		App->scene->selectedByHierarchy->drawGizmo();
	}
}

void ModuleRender::drawSceneView()
{
	ImVec2 size = ImGui::GetWindowSize();
	ImVec2 pos = ImGui::GetWindowPos();

	beginRenderTexture(size.x, size.y, &sceneFBO, &sceneTexture, &sceneRBO);

	glViewport(0, 0, size.x, size.y);
	glClearColor(sceneClearColor.x, sceneClearColor.y, sceneClearColor.z, sceneClearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &App->editorCamera->cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &App->editorCamera->cam->projectionMatrix[0][0]);

	//Todo: Update this thing with mesh gameobjects
	//App->modelLoader->DrawAll(App->programShader->defaultProgram);

	drawGameObjectsByFrustumCulling(App->programShader->defaultProgram, App->editorCamera->cam);

	drawAllBoundingBoxes();
	renderGrid(App->editorCamera->cam);
	ImGui::Image((void*)sceneTexture, ImVec2(size.x, size.y - 40), ImVec2(0, 1), ImVec2(1, 0));

	drawGizmos(pos.x, pos.y, size.x, size.y);

	App->debugDraw->Draw(App->editorCamera->cam, sceneFBO, App->window->width, App->window->height);
	endRenderTexture();
}

void ModuleRender::drawGameView(ComponentCamera* cam)
{
	ImVec2 size = ImGui::GetWindowSize();

	beginRenderTexture(size.x, size.y, &gameFBO, &gameTexture, &gameRBO);

	glViewport(0, 0, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	glClearColor(cam->clearColor.x, cam->clearColor.y, cam->clearColor.z, cam->clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &cam->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &cam->projectionMatrix[0][0]);

	drawGameObjectsByFrustumCulling(App->programShader->defaultProgram, cam);

	if(cam->selectedClearMode == SKYBOX) skybox->draw(cam);

	ImGui::Image((void*)gameTexture, ImVec2(size.x, size.y - 40), ImVec2(0, 1), ImVec2(1, 0));
	endRenderTexture();
}

void ModuleRender::drawSceneRenderSettings()
{
	ImGui::ColorEdit3("Clear Color", &sceneClearColor.x);
	
	ImGui::Separator();

	ImGui::Checkbox("Uses grid?", &usesGrid);
	if (usesGrid) ImGui::ColorEdit3("Grid Color", &gridColor.x);

	ImGui::Separator();

	ImGui::Checkbox("Show AABB Tree?", &renderAABBTree);
	if (renderAABBTree) ImGui::ColorEdit3("AABB Tree Color", &AABBColor.x);
}

//TODO: Update this method with proper gameobjects
void ModuleRender::drawGameObjects(GLuint program)
{
	for (size_t i = 0; i < App->scene->gameObjects.size(); i++)
	{
		if (((ComponentCamera*)App->scene->mainCamera->GetComponent(CAMERA))->AABBWithinFrustum(App->modelLoader->myBoundingBox) != OUTSIDE)
		{
			ComponentTransform * transform = (ComponentTransform*)App->scene->gameObjects.at(i)->myTransform;
			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &transform->getGlobalMatrix()[0][0]);

			App->scene->gameObjects.at(i)->Draw(program);
		}
	}
}

void ModuleRender::drawGameObjectsByFrustumCulling(GLuint program, ComponentCamera* cam)
{
	drawTreeNodeByFrustumCulling(program, cam, App->spacePartition->tree->root);
}

void ModuleRender::drawTreeNodeByFrustumCulling(GLuint program, ComponentCamera* cam, AABBTreeNode* node)
{
	if (node == nullptr) return;

	if (cam->AABBWithinFrustum(*node->box) != OUTSIDE)
	{
		if (node->gameObjectID != "")
		{
			ComponentTransform* transform = App->scene->findById(node->gameObjectID)->myTransform;

			glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &transform->getGlobalMatrix()[0][0]);
			transform->myGameObject->Draw(program);
		}
		else
		{
			drawTreeNodeByFrustumCulling(program, cam, node->leftChild);
			drawTreeNodeByFrustumCulling(program, cam, node->rightChild);
		}
	}
}
