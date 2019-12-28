#ifndef __ModuleProgramShader_H__
#define __ModuleProgramShader_H__

#include "Globals.h"
#include "Module.h"

#include <iostream>

class ModuleProgramShader :	public Module
{
public:
	ModuleProgramShader();
	~ModuleProgramShader();
	bool Init();
	bool Start();
	bool CleanUp();

	GLuint createProgram(const char*, const char*);
	GLuint compileShader(const GLuint type, const std::string source);
	std::string parseShader(const std::string& filepath);

public:
	GLuint defaultProgram;
	GLuint gridProgram;
	GLuint skyboxProgram;
};

#endif __ModuleProgramShader_H__