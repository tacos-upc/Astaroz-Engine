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

	//IL renderer
	ilutRenderer(ILUT_OPENGL);

	//Image
	ilGenImages(1, &myTextureId);
	ilBindImage(myTextureId);


	/* THIS PART WAS FOR THE FIRST EXERCISE WITH 'LENNA' IMAGE
	
	//Image
	ilGenImages(1, &myLena);
	ilBindImage(myLena);

	//Load image
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
	*/
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
	//Once assigned, we can 'unbind' the image
	ilDeleteImages(1, &myTextureId);

	return true;
}

Texture ModuleTexture::LoadTexture(const char* path)
{
	Texture texture;
	if (!ilLoadImage(path))
	{
		//Errors
		ILenum Error = ilGetError(); //Take the error code
		const char* string_error = iluErrorString(IL_NO_ERROR); //-->iluErrorString is not working correctly and throws exception. Get the error code and look for it in DevIL documentation
	}
	iluGetImageInfo(&myImageInfo);
	if (myImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage();

	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.path = path;

	return texture;
}