#pragma once
#include <windows.h>
#include <stdio.h>
#include "glew.h"

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

//Enums
enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

enum EventWindow
{
	WE_QUIT = 0,
	WE_HIDE = 1,
	WE_SHOW = 2,
	WE_COUNT
};

enum KeyState
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

// Configuration -----------
#define SCREEN_WIDTH 1024 //640 original
#define SCREEN_HEIGHT 960 //480 original
#define FULLSCREEN false
#define RESIZABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"
#define MAX_KEYS 300
#define SCREEN_SIZE 2


//Paths
#define TEXTURES_PATH "textures/"
#define MODELS_PATH "models/"
#define MODEL_BAKER_PATH "BakerHouse"
#define MODEL_BUNNY "ZomBunny"
#define MODEL_CLOCK "Clock"
#define MODEL_DOLLHOUSE "Dollhouse"
#define MODEL_DRAWERS "Drawers"
#define MODEL_FIRETRUCK "Firetruck"
#define MODEL_FLOOR "Floor"
#define MODEL_HEARSE "Hearse"
#define MODEL_PLAYER "Player"
#define MODEL_ROBOT "Robot"
#define MODEL_WALL "Wall"
#define MODEL_SPINNINGTOP "SpinningTop"

//help for texture paths
#define DIFFUSE "Diffuse"
#define SPECULAR "Specular"
#define OCCLUSION "Occlusion"
#define EMISSIVE "Emissive"

#define PNG ".png"
#define TIF ".tif"
#define FBX ".fbx"

//Utils
#define MAX(x,y) ((x>y) ? x:y)
#define MIN(x,y) ((x<y) ? x:y)


#define OPENGL_LOG_INIT(format, ...) log( __FILE__, __LINE__, format, __VA_ARGS__);
#define OPENGL_LOG_ERROR(format, ...) log( __FILE__, __LINE__, format, __VA_ARGS__);
void log( const char file[], const int line, const char* format, ...);