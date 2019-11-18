#include "ModuleTexture.h";


ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{

}

bool ModuleTexture::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilGenImages(1, &Lena);
	ilBindImage(Lena);
	if (!ilLoadImage("textures/lenna.png"))
		LOG("Failed loading image");
	ILenum Error;
	Error = ilGetError();
	Width = ilGetInteger(IL_IMAGE_WIDTH);
	Height = ilGetInteger(IL_IMAGE_HEIGHT);
	ilutRenderer(ILUT_OPENGL);
	Data = ilGetData();
	Texture = ilutGLBindTexImage();
	ilDeleteImages(1, &Lena);
	return true;
}

update_status ModuleTexture::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleTexture::PostUpdate()
{
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleTexture::CleanUp()
{
	// Cleanup
	return true;
}