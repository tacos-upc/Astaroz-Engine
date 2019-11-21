#pragma once
#include <windows.h>
#include <stdio.h>
#include "glew.h";
#include "Math/float2.h";
#include "Math/float3.h";

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

//Texture and Vertex to be used in different modules just including this generic file
struct Texture {
	unsigned int id;
	char*  type;
	const char* path;
	int width;
	int height;
	unsigned char *data;
};
struct Vertex {
	float3 Position;
	float3 Normal;
	float2 TexCoords;
	float3 Tangent;
	float3 Bitangent;
};

// Configuration -----------
#define SCREEN_WIDTH 1280 //640 original
#define SCREEN_HEIGHT 960 //480 original
#define FULLSCREEN false
#define VSYNC true
#define TITLE "Super Awesome Engine"