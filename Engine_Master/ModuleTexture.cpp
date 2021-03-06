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

	LOG("Trying to load a texture");
	loaded = false;

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
						texture.path = fullPath;	//textures path
						loaded = true;
					}
				}
			}
			else
			{
				//This means texture was loaded in step 2
				LOG("Texture path correct. Loaded in step 2, using '/models' directory\n");
				texture.path = fullPath;	//models path
				loaded = true;
			}
		}
	}
	else
	{
		//This means texture was loaded in step 1
		LOG("Texture path correct. Loaded in step 1, using the directory described in FBX\n");
		texture.path = path;	//original path
		loaded = true;
	}

	//Store image info in our member value 'myImageInfo'
	iluGetImageInfo(&myImageInfo);

	//Check if the texture is correctly oriented
	if (myImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		iluFlipImage(); //If not, flip the texture

	//Fill all the texture information that will be returned
	texture.id = ilutGLBindTexImage();
	texture.width = ilGetInteger(IL_IMAGE_WIDTH);
	texture.height = ilGetInteger(IL_IMAGE_HEIGHT);
	texture.data = ilGetData();
	texture.loaded = loaded;
	//If loaded flag is false, we must asign a texture.path anyway so we will use the received as parameter in this method
	if (!loaded)
		texture.path = path;
	
	return texture;
}

void ModuleTexture::loadSkybox(const char* path, int index)
{
	ILuint img;
	ilGenImages(1, &img);
	ilBindImage(img);

	ilLoadImage(path);
	ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);

	ILinfo ImageInfo;
	iluGetImageInfo(&ImageInfo);
	if (!ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) iluFlipImage();

	ILubyte* data = ilGetData();

	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int heigth = ilGetInteger(IL_IMAGE_HEIGHT);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		ilDeleteImages(1, &img);
	}

	return;
}
