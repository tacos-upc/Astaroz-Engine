#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "Skybox.h"
#include "SDL.h"
#include <list>
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_sdl.h"
#include "ImGUI/imgui_impl_opengl3.h"


struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

struct RenderTexture
{
	GLuint frameBuffer;
	GLuint rgbBuffer;
	GLuint depthBuffer;
};

class ModuleRender : public Module
{
public:
	ModuleRender();
	~ModuleRender();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	SDL_GLContext glcontext;
	std::list<RenderTexture> renderTextures;

	void renderGrid();
	void drawSceneView();
	void drawSceneRenderSettings();

private:

	GLuint fbo; //ID of the FrameBufferObject to render to texture
	GLuint texture; //ID of the texture to render in a window later
	GLuint rbo; //ID of the render buffer object for depth testing
	Skybox * skybox;

	//Scene render settings
	ImVec4 sceneClearColor;
	bool usesGrid = true;
	ImVec4 gridColor;

	void generateBuffers();
	bool beginRenderTexture(int, int);
	bool endRenderTexture();
};

#endif __ModuleRender_H__