#include "ModuleProgramShader.h"

#include <fstream>
#include <sstream>


ModuleProgramShader::ModuleProgramShader()
{
}


ModuleProgramShader::~ModuleProgramShader()
{
}

bool ModuleProgramShader::Init()
{
	return true;
}

bool ModuleProgramShader::Start()
{
	defaultProgram = createProgram("shaders/vertexShader.vs", "shaders/fragmentShader.fs");	//assigns myProgram member a value
	gridProgram = createProgram("shaders/gridShader.vs", "shaders/gridShader.fs");
	skyboxProgram = createProgram("shaders/skyboxShader.vs", "shaders/skyboxShader.fs");

	return true;
}

GLuint ModuleProgramShader::createProgram(const char* vertexShaderPath, const char*  fragmentShaderPath)
{
	GLuint program;
	//we need to create a program with a vertex shader and a fragment shader
	program = glCreateProgram();

	//create first our vertex and fragments shaders
	std::string vsSource = parseShader(vertexShaderPath);
	std::string fsSource = parseShader(fragmentShaderPath);
	GLuint vs = compileShader(GL_VERTEX_SHADER, vsSource);
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fsSource);

	//we can link them now with our program
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	//once shaders are linked into a program, we can delete them
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

std::string ModuleProgramShader::parseShader(const std::string& filepath)
{
	std::string shaderCode;
	std::ifstream stream;
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);	//this ensures ifstream object can throw exceptions
	try
	{
		stream.open(filepath);	//open file
		std::stringstream ss;	//we need a string stream variable to store buffer's content
		ss << stream.rdbuf();	//read file's buffer contents into streams
		stream.close();			//close file handlers
		shaderCode = ss.str();	//convert stream into string
	}
	catch (std::ifstream::failure e)
	{
		//LOG(e.code);
		LOG("There was an error parsing the shader!");
	}
	return shaderCode;
}

GLuint ModuleProgramShader::compileShader(const GLuint type, const std::string source)
{
	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	//check for errors
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		char message[512];
		glGetShaderInfoLog(shader, 512, nullptr, message);
		LOG(message);	//print any error message in the console log

		//delete shader in case of errors
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

// Called before quitting
bool ModuleProgramShader::CleanUp() 
{
	//delete our program
	glDeleteProgram(defaultProgram);

	return true;
}
