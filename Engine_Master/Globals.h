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
#define MODEL_BAKER_PATH "models/Player.fbx"
#define MODEL_BUNNY "models/Zombunny.fbx"

//Utils
#define MAX(x,y) ((x>y) ? x:y)
#define MIN(x,y) ((x<y) ? x:y)


#define OPENGL_LOG_INIT(format, ...) log( __FILE__, __LINE__, format, __VA_ARGS__);
#define OPENGL_LOG_ERROR(format, ...) log( __FILE__, __LINE__, format, __VA_ARGS__);
void log( const char file[], const int line, const char* format, ...);