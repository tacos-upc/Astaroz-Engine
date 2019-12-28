#pragma once
#include "Globals.h"

class SkyBoxTexture
{
public:

	SkyBoxTexture(const char* Directory,
		const char* PosXFilename,
		const char* NegXFilename,
		const char* PosYFilename,
		const char* NegYFilename,
		const char* PosZFilename,
		const char* NegZFilename);

	~SkyBoxTexture();

	bool Load();

	void Bind(GLenum TextureUnit);

private:

	const char* m_fileNames[6];
	GLuint m_textureObj;
};

