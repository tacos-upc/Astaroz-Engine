#include "ModuleTexture.h"

#include "glew.h"



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
	//ilGenImages(1, &myTextureId);
	//ilBindImage(myTextureId);

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

Texture* ModuleTexture::LoadTexture(const char* path)
{
	const char* npath= nullptr;
	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);
	
	LOG("Trying to load a texture");
	bool loaded = false;

	//Step 1 to find the texture
	LOG("Step 1: Trying to load texture from path described in the FBX...");
	if (!ilLoadImage(path))	//Check if the loading process is a success or not
	{
		//Errors
		ILenum Error = ilGetError(); //Take the error code
		const char* string_error = iluErrorString(Error); //-->iluErrorString is not working correctly and throws exception. Get the error code and look for it in DevIL documentation
		LOG(path);
		LOG(string_error);
		if (Error == 1290)	//IL_COULD_NOT_OPEN_FILE == not found there
		{
			//Step 2 to find the texture
			LOG("Texture not found in step 1");
			LOG("Step 2: Trying to load it from models directory...");
			std::string aux = MODELS_PATH + (std::string) path;		// --> "models/" + path
			const char* fullPath = aux.c_str();
			if (!ilLoadImage(fullPath))
			{
				Error = ilGetError();
				if (Error == 1290)	//IL_COULD_NOT_OPEN_FILE == not found there
				{
					//Step 3 to find the texture
					LOG("Texture not found in step 2");
					LOG("Step 3: Trying to load it from textures directory...");
					aux = TEXTURES_PATH + (std::string) path;		// --> "textures/" + path
					fullPath = aux.c_str();
					if (!ilLoadImage(fullPath))
					{
						Error = ilGetError();
						if (Error == 1290)	//IL_COULD_NOT_OPEN_FILE == not found there
							LOG("Texture not found in step 3 either...");
					}
					else
					{
						//This means texture was loaded in step 3
						LOG("Texture path correct. Loaded in step 3, using '/textures' directory\n");
						npath = fullPath;	//textures path
						loaded = true;
					}
				}
			}
			else
			{
				//This means texture was loaded in step 2
				LOG("Texture path correct. Loaded in step 2, using '/models' directory\n");
				npath = fullPath;	//models path
				loaded = true;
			}
		}
	}
	else
	{
		//This means texture was loaded in step 1
		LOG("Texture path correct. Loaded in step 1, using the directory described in FBX\n");
		npath = path;	//original path
		loaded = true;
	}

	GLuint textureID = 0;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	//Store image info in our member value 'myImageInfo'
	iluGetImageInfo(&myImageInfo);

	//Check if the texture is correctly oriented
	if (myImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage(); //If not, flip the texture

	//Fill all the texture information that will be returned
	unsigned width = 0;
	unsigned height = 0;
	unsigned pixelDepth = 0;
	
	
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);
	pixelDepth = ilGetInteger(IL_IMAGE_DEPTH);
	ILubyte* data = ilGetData();
	int format = 0;
	format = ilGetInteger(IL_IMAGE_FORMAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//If loaded flag is false, we must asign a texture.path anyway so we will use the received as parameter in this method
	if(!loaded)
		npath = path;

	ilDeleteImages(1, &image);
	glBindTexture(GL_TEXTURE_2D, 0);
	Texture* texture = new Texture(textureID, width, height, npath);
	
	return texture;
}