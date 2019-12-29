#ifndef __ModuleRender_H__
#define __ModuleRender_H__

#include "Module.h"
#include "Globals.h"
#include "Skybox.h"
#include "SDL.h"
#include <list>


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

	//Returns the array index 
	int generateFBO(unsigned int, unsigned int);
	void renderToTexture(unsigned int, unsigned int);

public:
	SDL_GLContext glcontext;
	std::list<RenderTexture> renderTextures;

	void renderGrid();

private:

	GLuint fbo; //ID of the FrameBufferObject to render to texture
	GLuint texture; //ID of the texture to render in a window later
	GLuint rbo; //ID of the render buffer object for depth testing
	Skybox * skybox;

	void generateBuffers();
	bool beginRenderTexture(int, int);
	bool endRenderTexture();

	void drawCameraWindow();
};

#endif __ModuleRender_H__