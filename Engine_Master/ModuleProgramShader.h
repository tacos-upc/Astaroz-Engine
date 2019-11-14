#pragma once
#include "Globals.h"
#include "Module.h"

#include <iostream>

class ModuleProgramShader :	public Module
{
public:
	ModuleProgramShader();
	~ModuleProgramShader();
	bool Init();
	bool CleanUp();

	void createProgram();
	GLuint compileShader(const GLuint type, const std::string source);
	std::string parseShader(const std::string& filepath);

public:
	GLuint myProgram;
};