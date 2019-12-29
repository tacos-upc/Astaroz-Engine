#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleModelLoader.h"
#include "ModuleProgramShader.h"
#include "Skybox.h"
#include "glew.h"

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
	// � check for errors
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
	glViewport(0, 0, App->window->width, App->window->height);

	generateBuffers();

	skybox = new Skybox("textures/skybox/sides.png", "textures/skybox/sides.png", "textures/skybox/top.png", "textures/skybox/bottom.png", "textures/skybox/sides.png", "textures/skybox/sides.png");

	return true;
}

update_status ModuleRender::PreUpdate()
{
	//Program (shaders: vertex shader + fragment shader)
	glUseProgram(App->programShader->defaultProgram);
	
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, -4.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "view"), 1, GL_TRUE, &App->editorCamera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(App->programShader->defaultProgram, "proj"), 1, GL_TRUE, &App->editorCamera->projectionMatrix[0][0]);

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
	drawCameraWindow();
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

	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &texture);
	glDeleteRenderbuffers(1, &rbo);

	//Destroy window is done in ModuleWindow
	SDL_GL_DeleteContext(glcontext);

	return true;
}

int ModuleRender::generateFBO(unsigned int width, unsigned int height)
{
	RenderTexture renderTexture;
	renderTexture.frameBuffer = 0;

	glGenFramebuffers(1, &renderTexture.frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, renderTexture.frameBuffer);


	// The texture we're going to render to
	glGenTextures(1, &renderTexture.rgbBuffer);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderTexture.rgbBuffer);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	// The depth buffer
	glGenRenderbuffers(1, &renderTexture.depthBuffer);

	glBindRenderbuffer(GL_RENDERBUFFER, renderTexture.depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderTexture.depthBuffer);


	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture.rgbBuffer, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


	// Check that framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		renderTextures.push_back(renderTexture);
		return renderTextures.size() -1;
	}
	return -1;
}

void ModuleRender::renderToTexture(unsigned int  width, unsigned int height)
{
	int renderTextureIndex = generateFBO(width, height);
	
	std::list<RenderTexture>::iterator it = renderTextures.begin();
	std::advance(it, renderTextureIndex);
	
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, (*it).frameBuffer);
	glViewport(0, 0, width, height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

	//layout(location = 0) out vec3 color;
}

void ModuleRender::renderGrid()
{
	GLuint gridProgram = App->programShader->gridProgram;
	glUseProgram(gridProgram);

	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f), float3x3::RotateY(math::pi / 4.0f), float3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "model"), 1, GL_TRUE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "view"), 1, GL_TRUE, &App->editorCamera->viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(gridProgram, "proj"), 1, GL_TRUE, &App->editorCamera->projectionMatrix[0][0]);

	

	//Grid
	glLineWidth(1.0f);
	float d = 35.0f;
	glBegin(GL_LINES);
	for (float i = -d; i <= d; i += 1.0f)
	{
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}
	glEnd();

	//Axis
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	// red X
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
	glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);
	// green Y
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
	glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);
	// blue Z
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
	glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
	glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);
	glEnd();
	glLineWidth(1.0f);

	glUseProgram(App->programShader->defaultProgram);
}

void ModuleRender::generateBuffers()
{
	glGenFramebuffers(1, &fbo);
	glGenTextures(1, &texture);
	glGenRenderbuffers(1, &rbo);
}


bool ModuleRender::beginRenderTexture(int width, int height)
{
	//Generate the frame buffer and bind it
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	//Create the texture to fill with the created framebuffer
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Attach the texture to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	//Render buffer for depth testing
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	//Atach the render buffer object to the default render buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

bool ModuleRender::endRenderTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	return true;
}

void ModuleRender::drawCameraWindow()
{
	bool active = false;
	ImGui::Begin("Scene", &active, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

	ImVec2 size = ImGui::GetWindowSize();
	//App->editorCamera->SetAspectRatio((int)size.y);

	beginRenderTexture(size.x, size.y);

	glViewport(0, 0, App->window->width, App->window->height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draw program shader
	App->modelLoader->Draw(App->programShader->defaultProgram);
	renderGrid();

	skybox->draw();

	ImGui::GetWindowDrawList()->AddImage(
		(void *)texture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x, ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);


	ImGui::End();

	endRenderTexture();
}
