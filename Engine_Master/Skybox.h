#pragma once
#include "Globals.h"
#include <list>

class Skybox
{
public:

	Skybox(
		const char* PosXFilename,
		const char* NegXFilename,
		const char* PosYFilename,
		const char* NegYFilename,
		const char* PosZFilename,
		const char* NegZFilename);

	~Skybox();

	GLuint Load();
	void draw();

private:

	std::list<const char*> fileNames;
	GLuint texture;
	GLuint vbo;
	GLuint vao;
};
