#include "ModuleTexture.h";

#include "glew.h";


ModuleTexture::ModuleTexture()
{

}

ModuleTexture::~ModuleTexture()
{

}

bool ModuleTexture::Init()
{
	//Init IL
	ilInit();
	iluInit();
	ilutInit();

	//Image
	ilGenImages(1, &myLena);
	ilBindImage(myLena);
	if (!ilLoadImage("textures/lenna.png"))
	{
		//Errors
		ILenum err = ilGetError();
		LOG("Failed loading image");
	}

	//Il renderer
	ilutRenderer(ILUT_OPENGL);

	//Assign member variables
	myData = ilGetData();
	myTexture = ilutGLBindTexImage();

	//Once assigned, we can 'unbind' the image
	ilDeleteImages(1, &myLena);

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

Texture ModuleTexture::LoadTexture(const char* path)
{
	Texture texture;
	//LOG(path);
	if (!ilLoadImage("Baker_house.png"))
	{
		//Errors
		ILenum Error = ilGetError();

		const char* string_error = iluErrorString(IL_NO_ERROR);

		//LOG();
	}
	//ilLoadImage(path);
	iluGetImageInfo(&imageInfo);
	if (imageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.path = path;

	return texture;
}