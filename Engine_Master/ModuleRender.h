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
#include "ComponentCamera.h"


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

	void renderGrid(ComponentCamera*);
	void drawSceneView();
	void drawGameView();
	void drawSceneRenderSettings();
	void drawGameObjects(GLuint);

private:

	GLuint sceneFBO; //ID of the FrameBufferObject to render to texture
	GLuint sceneTexture; //ID of the texture to render in a window later
	GLuint sceneRBO; //ID of the render buffer object for depth testing

	GLuint gameFBO;
	GLuint gameTexture;
	GLuint gameRBO;

	Skybox * skybox;

	//Scene render settings
	ImVec4 sceneClearColor;
	bool usesGrid = true;
	ImVec4 gridColor;
	bool renderAABBTree = false;
	ImVec4 AABBColor;
	bool drawAll;
	bool drawSpacePartition;

	void generateBuffers(GLuint*, GLuint*, GLuint*);
	bool beginRenderTexture(int, int, GLuint*, GLuint*, GLuint*);
	bool endRenderTexture();
	void drawAllBoundingBoxes();
	void drawGizmos(float, float, float, float);
};

#endif __ModuleRender_H__